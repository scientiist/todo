// TodoApp.h
// Implementation of TodoList Console App.
// Reads input, processes arguments and options, and outputs results.
// @auth J. O'Leary
// @edit 12 April 2023
// @copyright Conarium Software LLC
// @license GPL3

#pragma once
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "colors.h"
#include "json.h"

#define VERSION "1.2"
#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"

using std::chrono_literals::operator""h;
using timestamp = std::chrono::time_point<std::chrono::system_clock>;


std::string timestamp_to_string(const timestamp time)
{
	auto in_time_t = std::chrono::system_clock::to_time_t(time);
	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), TIMESTAMP_FORMAT);
	return ss.str();
}

const timestamp string_to_timestamp(std::string time_str)
{
	std::tm tm = {};
	strptime(time_str.c_str(), TIMESTAMP_FORMAT, &tm);
	// the -1h is a lazy hack to hotfix timezones incrementing the hour
	return std::chrono::system_clock::from_time_t(std::mktime(&tm))-1h;
}

timestamp get_current_timestamp()  { return std::chrono::system_clock::now(); }

std::string get_current_timestamp_str() {
	return timestamp_to_string(std::chrono::system_clock::now());
}

int get_timestamp_age(const std::string timestamp)
{
	auto tp = string_to_timestamp(timestamp);
	const auto now = std::chrono::system_clock::now();
	const auto difference = std::chrono::duration_cast<std::chrono::days>(now-tp).count();
	return difference;
}

bool matches(const std::string needle, std::vector<std::string> haystack)
{
	return std::find(haystack.begin(), haystack.end(), needle) != haystack.end();
}

std::string rebuild_args_to_sentence(std::vector<std::string> args)
{
	std::string sentence;
	for (int i = 0; i < args.size(); i++) {
		sentence += args[i];
		if (i < args.size()-1) // Append space between each token except for last
			sentence += " ";
	}
}

#define FILE_NAME ".todo.txt"
#define CONFIG_NAME ".todo.conf"
#define MAGENTA_LIMIT 21
#define RED_LIMIT 14
#define YELLOW_LIMIT 7
#define GREEN_LIMIT 1

class TodoApp
{
public:
	std::string working_dir;
	std::string home_dir;
	std::string file_path;
	std::string config_path;
	std::vector<std::string> args;
	std::string current_timestamp;
	std::string tag = "";
	TodoApp(int argc, char ** argv);
	void Run();
	void ShowVersionInfo();
	void ShowLicenseInfo();
	void ShowHelpInfo();
	void AddTodo();
	void OutputTodoList();
	void OutputLine(const char*, std::string, std::string, std::string, std::string);
protected:
private:
	bool ParseOptions();
};

TodoApp::TodoApp(int argc, char ** argv)
{
	home_dir = getenv("HOME");
	config_path = home_dir + "/" + CONFIG_NAME;
	file_path = home_dir + "/" + FILE_NAME;
	if (argc > 1)
		args = std::vector<std::string>(argv+1, argv+argc);
	current_timestamp = get_current_timestamp_str();
}

// Returns whether to terminate early or not;
bool TodoApp::ParseOptions()
{
	// Check for option flags to parse out (--<flag>)
	// flags are removed from args_list at this point
	// runs in reverse so we can remove vector elements safely
	for (int i = args.size()-1; i>=0; i--) {
		if (matches(args[i], {"--version", "-v", "--v"})) {
			this->ShowVersionInfo();
			return true;
		}
		if (matches(args[i], {"--help", "-h", "--h"})) {
			this->ShowHelpInfo();
			return true;
		}
		if (matches(args[i], {"--license", "-l"})) {
			this->ShowLicenseInfo();
			return true;
		}
		if (matches(args[i], {"--here"})) {
			this->file_path = FILE_NAME;
			args.erase(args.begin()+i);
			continue;
		}
		if (matches(args[i], {"--urgent"}))     {
			this->tag = "URGENT";
			args.erase(args.begin()+i);
			continue;
		 }
		if (matches(args[i], {"--important"}))  {
			this->tag = "IMPORTANT";
			args.erase(args.begin()+i);
			continue;
		}
		if (matches(args[i], {"--tag"}))        {
			this->tag = args[i+1]; // TODO: Bounds check
			args.erase(args.begin()+i);
			args.erase(args.begin()+i);
			continue;
		}
		if (matches(args[i], {"--file", "-f"})) {
			this->file_path = args[i+1];
			args.erase(args.begin()+i);
			args.erase(args.begin()+i);
			continue;
		}
		if (matches(args[i], {"--config"}))     {
			this->config_path = args[i+1]; // TODO: Bounds check
			args.erase(args.begin()+i);
			args.erase(args.begin()+i); // remove 2 indices
			continue;
		}
	}
	return false; // No special conditions met, so we continue the routine;
}

void TodoApp::Run()
{
	if (this->ParseOptions())
		return;

	if (this->args.size() == 0)
		this->OutputTodoList();
	else
		this->AddTodo();
}


void TodoApp::AddTodo()
{
	// Take whatever args we have left
	// & Append into a sentence
	std::string text = rebuild_args_to_sentence(this->args);

	nlohmann::json token = {
		{"text", text}, 
		{"timestamp", this->current_timestamp},
		{"tag", this->tag},
	};
	std::ofstream file(this->file_path, std::ios_base::app);
	file << token.dump() << std::endl;
	file.close();
	std::cout << "Added to todo list: " << text << std::endl;
}


void TodoApp::OutputLine(const char* colorcode, std::string timestamp, std::string age, std::string text, std::string tag)
{
	std::cout << BOLD << colorcode;
	std::cout << "[" << timestamp << "] [" << age << "] ";
	if (tag.size() > 0) {
		if (tag == "URGENT")
			std::cout << FG_RED;
		if (tag == "IMPORTANT")
			std::cout << FG_YELLOW;
		std::cout << "(" << tag << ")";
	}
	std::cout << FG_WHITE << text << std::endl;
}

void TodoApp::OutputTodoList()
{
	std::ifstream file(this->file_path);

	if (file.peek() == std::ifstream::traits_type::eof())
	{
		std::cout << "No tasks on the TODO list!" << std::endl;
		return;
	}

	std::string str;
	// Read each line from the file and format for output
	while (std::getline(file, str)) {
		// Parses 1 line of the file as a json object;
		// NOT the entire file
		// Grab relevant strings;
		nlohmann::json token = nlohmann::json::parse(str);
		std::string timestamp = token["timestamp"].get<std::string>();
		std::string text      = token["text"     ].get<std::string>();
		std::string tag       = token["tag"      ].get<std::string>();
		// Calculate age of todo
		// TODO: Configurable time (days, hours)
		int age = get_timestamp_age(timestamp);
		std::string age_as_string = std::to_string(age) + " days ago";

		auto colorcode = FG_BLUE;
		if (age > MAGENTA_LIMIT)      { colorcode = FG_MAGENTA; }
		else if (age > RED_LIMIT)     { colorcode = FG_RED;     }
		else if (age > YELLOW_LIMIT)  { colorcode = FG_YELLOW;  }
		else if (age > GREEN_LIMIT)   { colorcode = FG_GREEN;   }
		else                          { colorcode = FG_BLUE;    }

		this->OutputLine(colorcode, timestamp, age_as_string, text, tag);
	}
	file.close();
	std::cout << RESET_ALL;
}

void TodoApp::ShowVersionInfo()
{
	std::cout << "Todo List Version " << VERSION << std::endl;
}

void TodoApp::ShowLicenseInfo()
{
	std::cout << "TodoList Copyright (C) 2023 Conarium Software" << std::endl;
	std::cout << "This program comes with ABSOLUTELY NO WARRANTY." << std::endl;
	std::cout << "This is free software, and you are welcome to redistribute it." << std::endl;
	std::cout << "Licensed under GNU GPL-3. See LICENSE.txt for full license." << std::endl;
}

void TodoApp::ShowHelpInfo()
{
	std::cout << "Usage: todo [-flags] <message>" << std::endl;
	std::cout << "Flags:"      << std::endl;
	std::cout << "--help         Shows this help screen."         << std::endl;
	std::cout << "--version      Shows current version."          << std::endl;
	std::cout << "--license      Shows GPL3 Information."         << std::endl;
	std::cout << "--tag <tag>    Specify a tag on the reminder. " << std::endl;
	std::cout << "--important    Set tag to IMPORTANT"            << std::endl;
	std::cout << "--urgent       Set tag to URGENT"               << std::endl;
	std::cout << "--here         Use file in current directory"   << std::endl;
	std::cout << "--file <dir>   Specify todofile directory"      << std::endl;
	std::cout << "--config <dir> Specify configfile directory"    << std::endl;
}
