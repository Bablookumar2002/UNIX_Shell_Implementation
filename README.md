# Simple Shell Implementation

## Overview
This project is a simple shell implementation in C that supports basic shell functionality such as changing directories, executing commands, handling parallel and sequential commands, and command redirection. The shell can handle interrupt signals like `Ctrl+C` and `Ctrl+Z` gracefully and provides a prompt for user input.

## Features
1. **Basic Command Execution**: Execute standard shell commands.
2. **Change Directory**: Change the current working directory using the `cd` command.
3. **Parallel Command Execution**: Execute multiple commands in parallel using `&&`.
4. **Sequential Command Execution**: Execute multiple commands sequentially using `##`.
5. **Command Redirection**: Redirect output of a command to a file using `>`.

## Files and Directories
- `main.c`: Contains the implementation of the shell.
- `README.md`: Documentation of the project.

## Functions
- `changeDirectory(char **Token)`: Changes the current working directory.
- `parseInput(const char *input)`: Parses the user input into tokens (command and arguments).
- `executeCommand(char **Token)`: Executes a single command.
- `executeParallelCommands(char **Token)`: Executes commands in parallel.
- `executeSequentialCommands(char **Token)`: Executes commands sequentially.
- `executeCommandRedirection(char **Token)`: Executes a command and redirects its output to a file.
- `signalHandler(int sig)`: Handles `Ctrl+C` and `Ctrl+Z` signals.

## Usage
### Compiling the Code
To compile the code, use the following command:
```sh
gcc -o shell main.c
```

### Running the Shell
To run the shell, use the following command:
```sh
./shell
```

### Shell Commands
- **Basic Commands**: Any standard shell command like `ls`, `echo`, `pwd`, etc.
  ```sh
  ls
  pwd
  echo "Hello, World!"
  ```
- **Change Directory**: Use the `cd` command followed by the directory path.
  ```sh
  cd /path/to/directory
  ```
- **Parallel Command Execution**: Use `&&` to execute commands in parallel.
  ```sh
  command1 && command2 && command3
  ```
- **Sequential Command Execution**: Use `##` to execute commands sequentially.
  ```sh
  command1 ## command2 ## command3
  ```
- **Command Redirection**: Use `>` to redirect command output to a file.
  ```sh
  command > output.txt
  ```

### Exiting the Shell
To exit the shell, use the `exit` command.
```sh
exit
```

## Error Handling
The shell provides error messages for various error conditions such as:
- Memory allocation errors
- Fork errors
- Execution errors
- File open errors

## Signal Handling
The shell handles `Ctrl+C` and `Ctrl+Z` signals to provide a smooth user experience. Custom handlers for these signals ensure that the shell prompt is re-displayed after an interrupt.

## Debugging
A flag (`myflag`) is used for debugging purposes. Setting `myflag` to `0` enables debug prints throughout the code.

## License
This project is open-source and available under the Babloo License. Feel free to use, modify, and distribute it as per the license terms.
