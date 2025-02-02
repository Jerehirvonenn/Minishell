# Minishell

This is a custom shell implementation created as part of a group project. The shell replicates key functionalities of Unix-based command-line interpreters, allowing users to execute commands, manage processes, and navigate the file system

Project is made by [@vkuznets23](https://github.com/vkuznets23) and [@Jerehirvonenn](https://github.com/Jerehirvonenn)

## Feeatures
* Command execution (both built-in and external commands)
* Input and output redirection (`>`, `<`)
* Basic environment variable support

## Installation
``` bash
https://github.com/Jerehirvonenn/Minishell
cd Minishell
make
```

## Usage
Run the shell by executing:
```
./minishell
```

Once inside the shell, you can execute commands as you would in a normal Unix shell: `ls -la` `wc -l` and pipes like `ls -la | wc -l` etc.

## Built-in Commands
Our shell includes the following built-in commands:

* `cd` - Change directory
* `exit` - Exit the shell
* `env` - Print environment variables
* `echo` - Print a message or variable value
* `export` - Set an environment variable
* `unset` - Unset an environment variable

## Contribution
Contributions are welcome! If you spot any bugs, feel free to open an issue.
