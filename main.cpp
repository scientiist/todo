/// To Do List Program
// Copyright Conarium Software 2023
// @auth J. O'Leary

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

void output_line(const char* colorcode, std::string timestamp, std::string age, std::string text)
{
 std::cout << BOLD << colorcode  << "[" << timestamp << "] [" << age << "] "<< FG_WHITE  << text << std::endl;
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
		std::string ag = token["tag"].get<std::string>();
		// Calculate age (in days) of the TODO
		int age = get_timestamp_age(timestamp);
		// TODO: Make timeline configurable.
		// Create timestamp string
		std::string days_counter = std::to_string(age) + " days ago";

		if      (age > MAGENTA_LIMIT) { output_line(FG_MAGENTA, timestamp, days_counter, text); }
		else if (age > RED_LIMIT)     { output_line(FG_RED,     timestamp, days_counter, text); }
		else if (age > YELLOW_LIMIT)  { output_line(FG_YELLOW,  timestamp, days_counter, text); }
		else if (age > GREEN_LIMIT)   { output_line(FG_GREEN,   timestamp, days_counter, text); }
		else                          { output_line(FG_BLUE,    timestamp, days_counter, text); }
	}
	file.close();
	std::cout << RESET_ALL;
}

void append_to_list(std::string file_id, std::string current_timestamp, std::string content){
	nlohmann::json token = {
		{"text", content},
		{"timestamp", current_timestamp},
		{"tag", ""},
	};
	std::ofstream file(file_id, std::ios_base::app);
	file << token.dump() << std::endl;
	file.close();
	std::cout << "Added to TODO list!" << std::endl;
}

int main(int argc, char ** argv)
{
	// System Variables
	std::string current_timestamp = get_current_timestamp_str();
	std::string home_dir = getenv("HOME");
	std::string file_id = home_dir + "/" + FILENAME;

	std::vector<std::string> args_list(argv + 1, argv + argc);

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
	}

	// Run the program with cherry-picked args_list
	if (args_list.size() < 1) { 
		output_list(file_id);
       	} else {
		std::string item = rebuild_args_to_sentence(args_list);
		append_to_list(file_id, get_current_timestamp_str(), item);
		return 0;
	}
	return 0;
}
