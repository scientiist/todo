# TODO
A command-line todo-list and reminder program written in C++.

![Screenshot](screenshot.png?raw=true)

## Why?
I'm forgetful, and I find this to be a helpful tool.

## About
Written & Maintained by Josh O'Leary.

## Usage

`todo [--flags] <reminder_text>`

To list current TODOs, you can run the program without any arguments:

`todo`

By default, the program reads and writes to a file named `.todo.txt` located in the home directory.

You can use the current directory with the `--here` flag, or set a custom directory with the `--file <path>` flag.

To add a new entry, run the program with your message afterward:

`todo <blah blah blah>`

### Flags

Optional --flags are avaliable to use:

`todo --help`
`todo --version`
`todo --license`
`todo --here` - Read/Write from current directory file.
`todo --file [filepath]` - Sets custom filepath
`todo --config [configpath]` - Sets custom config path
`todo --tag [tag]` - Tags the input message

## Installation
The project is built with CMake.

1. Clone the repository:

`git clone https://github.com/scientiist/todo`

2. Move into the repository directory:

`cd todo`

3. Create a build directory, and enter it:

`mkdir build`

`cd build`

4. Run the CMake configure command, this will setup our build system.

`cmake ..`

4. Run the build system.

`cmake --build .`

5. If all succeeds, you'll have an executable in your build directory:

`./build/todo`

## Contributing
Feel free to submit issues and pull requests. I'll definitely take a look at them.

## Contact
Email: josh@conarium.software
