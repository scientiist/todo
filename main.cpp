// TODO list program //
// written and maintained by J. O'Leary
// Contact me: josh@conarium.software
// last revision: April 10, 2023
// Copyright Conarium Software LLC - Licensed under GPL3
// See README.txt for further information.

// Compilation //

// Usage //

// Changelog //
// April 11, 2023
// Refactored program logic into TodoApp class
// April 10, 2023
// added --urgent and --important flags
// added --tag <tag> flag
// implement tag display

// Source //
// See TodoApp.h for app implementation;
#include <iostream>
#include "TodoApp.h"

int main(int argc, char ** argv)
{
	TodoApp app(argc, argv);
	app.Run();
	return 0;
}
