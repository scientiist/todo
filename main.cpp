// TODO list program //
// written and maintained by J. O'Leary
// Contact me: josh@conarium.software
// last revision: April 10, 2023
// Copyright Conarium Software LLC - Licensed under GPL3
// See README.txt for further information.

// Compilation //

// Usage //

// Changelog //
// April 10, 2023
// added --urgent and --important flags
// added --tag <tag> flag
// implement tag display

// Source //
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>

#include "colors.h"
#include "json.h"

using std::chrono_literals::operator""h;
using string = std::string;
using timestamp = std::chrono::time_point<std::chrono::system_clock>;
string timestamp_to_string(const timestamp time)
{
    auto in_time_t = std::chrono::system_clock::to_time_t(time);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
const timestamp string_to_timestamp(std::string time_str)
{
    std::tm tm = {};
    strptime(time_str.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
    // The -1h is a lazy hack to hotfix timezones incrementing the hour
    return std::chrono::system_clock::from_time_t(std::mktime(&tm))-1h;
}
timestamp get_current_timestamp() {
	return std::chrono::system_clock::now();
}
string get_current_timestamp_str() {
    return timestamp_to_string(get_current_timestamp());
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

void show_help()
{
	std::cout << "To Do List Program by Conarium Software" << std::endl;
	std::cout << "Usage: todo " << std::endl;
}

std::string rebuild_args_to_sentence(std::vector<std::string> args)
{
	std::string sentence;
	for(int i = 0; i < args.size(); i++) {
		sentence += args[i];
		if (i < args.size()-1) // Append a space between each token, without appending to the final token's end
			sentence += " ";
	}
	return sentence;
}

#define FILENAME ".todo.txt"
#define MAGENTA_LIMIT 21
#define RED_LIMIT 14
#define YELLOW_LIMIT 7
#define GREEN_LIMIT 1

void output_line(const char* colorcode, std::string timestamp, std::string age, std::string text, std::string tag)
{
	std::cout << BOLD << colorcode  << "[" << timestamp << "] [" << age << "] ";
	if (tag.size() > 0)
	{
		if (tag == "URGENT")
			std::cout << FG_RED;
		if (tag == "IMPORTANT")
			std::cout << FG_YELLOW;

		std::cout << "(" << tag << ") ";
	}
	std::cout << FG_WHITE << text << std::endl;
}

void output_list(std::string file_path)
{
	// Find, Parse, Output the todo list
	std::ifstream file(file_path);

    	// is the file empty?
    	if (file.peek() == std::ifstream::traits_type::eof())
    	{
        	std::cout << "No tasks on the TODO list!" << std::endl;
        	return;
    	}
	// Read each line from the file and parse it into a TODO.
	std::string str;
	while (std::getline(file, str)) {
		// Parses 1 line of the file (str) as a json object; (NOT the entire file!)
		// Grab relevant strings; timestamp, text, tag
		nlohmann::json token = nlohmann::json::parse(str);
		std::string timestamp = token["timestamp"].get<std::string>();
		std::string text = token["text"].get<std::string>();
		std::string tag = token["tag"].get<std::string>();
		// Calculate age (in days) of the TODO
		int age = get_timestamp_age(timestamp);
		// TODO: Make timeline configurable.
		// Create timestamp string
		std::string days_counter = std::to_string(age) + " days ago";

		auto colorcode = FG_BLUE;
		if      (age > MAGENTA_LIMIT) { colorcode = FG_MAGENTA; }
		else if (age > RED_LIMIT)     { colorcode = FG_RED;     }
		else if (age > YELLOW_LIMIT)  { colorcode = FG_YELLOW;  }
		else if (age > GREEN_LIMIT)   { colorcode = FG_GREEN;   }
		else                          { colorcode = FG_BLUE;    }

		output_line(colorcode, timestamp, days_counter, text, tag);
	}
	file.close();
	std::cout << RESET_ALL;
}

void append_to_list(std::string file_id, std::string current_timestamp, std::string content, std::string tag){
	nlohmann::json token = {
		{"text", content},
		{"timestamp", current_timestamp},
		{"tag", tag},
	};
	std::ofstream file(file_id, std::ios_base::app);
	file << token.dump() << std::endl;
	file.close();
	std::cout << "Added to TODO list!" << std::endl;
}

// TODO: Refactor into class

int main(int argc, char ** argv)
{
	// System Variables
	std::string current_timestamp = get_current_timestamp_str();
	std::string home_dir = getenv("HOME");

	// Look for config
	std::string config_path = home_dir + "/" + ".todo.cfg.json";
	std::ifstream config(config_path);

	if (config.peek() == std::ifstream::traits_type::eof())
	{
		// No Config Yet, so we need to generate one
		// Presumably the first time the software is used
		// also show some help info?
	}

	std::string file_id = home_dir + "/" + FILENAME;
	std::vector<std::string> args_list(argv + 1, argv + argc);

	std::string tag = "";

	// Check for option flags to parse out. (--<flag>)
	// Flags are removed from args_list at this point.
	// Runs in reverse so we can remove vector elements safely.
	for(int i = args_list.size()- 1; i >= 0 ; i--) {
		if (matches(args_list[i], {"--version", "-v", "--v"})) {
			args_list.erase(args_list.begin() + i);
			std::cout << "Version 1.1" << std::endl;
			return 0; // Certain Flags cause the program to terminate.
		}
		if (matches(args_list[i], {"--help", "-h", "--h"})) {
			args_list.erase(args_list.begin() + i);
			show_help();
			return 0; // Certain Flags cause the program to terminate.
		}
		if (matches(args_list[i], {"--here"})) {
			args_list.erase(args_list.begin() + i);

			file_id = FILENAME;
           		continue;
		}
		if (matches(args_list[i], {"--urgent"}))
		{
			args_list.erase(args_list.begin()+i);
			tag = "URGENT";
			continue;
		}
		if (matches(args_list[i], {"--important"}))
		{
			args_list.erase(args_list.begin()+i);
			tag = "IMPORTANT";
			continue;
		}
		if (matches(args_list[i], {"--tag"}))
		{
			tag = args_list[i+1];
			args_list.erase(args_list.begin()+i);
			args_list.erase(args_list.begin()+i);
			continue;
		}
	}

	// Run the program with cherry-picked args_list
	if (args_list.size() < 1) {
		output_list(file_id);
       	} else {
		std::string item = rebuild_args_to_sentence(args_list);
		append_to_list(file_id, get_current_timestamp_str(), item, tag);
		return 0;
	}
	return 0;
}
