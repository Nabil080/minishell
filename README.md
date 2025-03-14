# Minishell - Lightweight Unix Shell

## About

**Minishell** is a minimalist Unix shell inspired by Bash. It allows users to execute commands, handle environment variables, and use common shell features such as pipes and redirections. 

// TODO: demo gif

This project was made in collaboration with [@corentin-ltc](https://github.com/corentin-ltc/)

## Features

- **Execute binaries** via absolute paths or `$PATH`
- **Built-in commands**: `cd`, `echo`, `pwd`, `env`, `export`, `unset`, `exit`
- **Pipes (`|`) and redirections** (`<`, `>`, `>>`, `<<`)
- **Environment variable expansion** (`$VAR`)
- **Heredoc (`<<`) support**
- **Signal handling** (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)

## Dependencies

- Linux
- A C compiler (`gcc` or `clang`)
- `make` (for building the project)
- `libreadline-dev`

## Installation

Install dependencies:
```sh
sudo apt install libreadline-dev
```

Clone and compile:
```sh
git clone https://github.com/Nabil080/minishell
cd minishell
make
```
The Makefile will automatically pull and compile the required submodules.

Run Minishell:
```sh
./minishell
```

## Controls

- **Enter a command** → Execute it
- **Ctrl+D** → Exit the shell
- **Ctrl+C** → Interrupt the current process
- **Ctrl+\\** → Quit blocking commands like `cat`

## Technical Details

Minishell processes commands in two main stages:
- **Parsing**: Tokenizing input, expanding variables, handling redirections, and preparing execution.
- **Execution**: Creating child processes, setting up pipes, and running commands.

For a deeper dive into the internals, check out [`TECHNICAL.md`](TECHNICAL.md).

## Cleaning Up

To remove compiled files, run:
```sh
make clean
```

To completely remove all binaries and object files:
```sh
make fclean
```
