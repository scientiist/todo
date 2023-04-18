# TODO
A command-line todo-list and reminder program written in C++.
Written & Maintained by Josh O'Leary.

![Screenshot](screenshot.png?raw=true)

## Why?
I'm forgetful, and I find this to be a helpful tool.

## Usage

`todo [--flags] <reminder_text>`

To list current TODOs, you can run the program without any arguments.
`todo`

By default, the program reads and writes to a file named `.todo.txt` located in the home directory.
You can use the current directory with the `--here` flag,
Or set a custom directory with the `--file <path>` flag.

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

## Compiling
The project is built with CMake.

Clone the repository:
`git clone https://github.com/scientiist/todo`
Move into the repository directory:
`cd todo`
Create a build directory:
`mkdir build`
Run the build system, pointed into the `build` directory.
`cmake --build ./build/`
If all succeeds, you'll have an executable in your build directory:
`./build/todo`

## Contributing
Feel free to submit issues and pull requests. I'll definitely take a look at them.

## Contact
Email: josh@stepbro.wtf
