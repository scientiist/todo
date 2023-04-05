// Console Color Codes

#define RESET_ALL "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define STANDOUT "\033[3m"
#define UNDERSCORE "\033[4m"
#define BLINK "\033[5m"
#define INVERT "\033[7m"
#define HIDDEN "\033[8m"
#define SAVE_SCREEN "\033[?47h"
#define RESTORE_SCREEN "\033[?47l"
#define BG_BLACK "\033[40m"
#define BG_DARK_GRAY "\033[1;40m"
#define BG_RED   "\033[41m"
#define BG_LIGHT_RED "\033[1;41m"
#define BG_GREEN "\033[42m"
#define BG_LIGHT_GREEN "\033[1;42m"
#define BG_BROWN "\033[43m"
#define BG_YELLOW "\033[1;43m"
#define BG_BLUE "\033[44m"
#define BG_LIGHT_BLUE "\033[1;44m"
#define BG_MAGENTA "\033[45m"
#define BG_LIGHT_PURPLE "\033[1;45m"
#define BG_CYAN "\033[46m"
#define BG_LIGHT_CYAN "\033[1;46m"
#define BG_LIGHT_GRAY "\033[47m"
#define BG_WHITE "\033[1;47m"
#define BG_DEFAULT "\033[48m"
#define FG_BLACK "\033[30m"
#define FG_DARK_GRAY "\033[1;30m"
#define FG_RED	"\033[31m"
#define FG_LIGHT_RED "\033[1;31m"
#define FG_GREEN "\033[32m"
#define FG_LIGHT_GREEN "\033[1;32m"
#define FG_BROWN "\033[33m"
#define FG_YELLOW "\033[1;33m"
#define FG_BLUE  "\033[34m"
#define FG_LIGHT_BLUE "\033[1;34m"
#define FG_MAGENTA "\033[35m"
#define FG_LIGHT_PURPLE "\033[1;35m"
#define FG_CYAN "\033[36m"
#define FG_LIGHT_CYAN "\033[1;36m"
#define FG_LIGHT_GRAY "\033[37m"
#define FG_WHITE "\033[1;37m"
#define FG_DEFAULT "\033[38m"

#include <iostream>

void fg_color_test(const char* bg)
{
	std::cout << bg;
	std::cout << FG_BLACK 		<< "Sample Text";
	std::cout << FG_DARK_GRAY 	<< "Sample Text";
	std::cout << FG_RED 		<< "Sample Text";
	std::cout << FG_LIGHT_RED 	<< "Sample Text";
	std::cout << FG_GREEN 		<< "Sample Text";
	std::cout << FG_LIGHT_GREEN 	<< "Sample Text";
	std::cout << FG_BROWN 		<< "Sample Text";
	std::cout << FG_YELLOW 		<< "Sample Text";
	std::cout << FG_BLUE 		<< "Sample Text";
	std::cout << FG_LIGHT_BLUE 	<< "Sample Text";
	std::cout << FG_MAGENTA 	<< "Sample Text";
	std::cout << FG_LIGHT_PURPLE 	<< "Sample Text";
	std::cout << FG_CYAN 		<< "Sample Text";
	std::cout << FG_LIGHT_CYAN 	<< "Sample Text";
	std::cout << FG_WHITE 		<< "Sample Text";
	std::cout << FG_LIGHT_GRAY 	<< "Sample Text";
	std::cout << std::endl;
}


void bg_color_test()
{
	fg_color_test(BG_BLACK);
	fg_color_test(BG_DARK_GRAY);
	fg_color_test(BG_RED);
	fg_color_test(BG_LIGHT_RED);
	fg_color_test(BG_GREEN);
	fg_color_test(BG_LIGHT_GREEN);
	fg_color_test(BG_BROWN);
	fg_color_test(BG_YELLOW);
	fg_color_test(BG_BLUE);
	fg_color_test(BG_LIGHT_BLUE);
	fg_color_test(BG_MAGENTA);
	fg_color_test(BG_LIGHT_PURPLE);
	fg_color_test(BG_CYAN);
	fg_color_test(BG_LIGHT_CYAN);
	fg_color_test(BG_WHITE);
	fg_color_test(BG_LIGHT_GRAY);
	std::cout << BG_DEFAULT << std::endl;
}

void color_test()
{
	bg_color_test();
	std::cout << RESET_ALL;
	std::cout << BOLD;
	bg_color_test();
	std::cout << RESET_ALL << std::endl;
}
