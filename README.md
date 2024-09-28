# Minishell

## Introduction

A simple shell based on Bash



This project is about recreating our own minimalist shell, taking bash as reference. This is a group project I had the chance to work on with [@corentin-ltc](https://github.com/corentin-ltc).
## Table of content

- [Introduction](#introduction)
- [How to Use It](#how-to-use-it)
    - [Requirements](#requirements)
    - [Installation](#installation)
- [What It Does](#what-it-does)
- [How It Works](#how-it-works)
    - [Parsing](#parsing)
        - [Checking the Input](#checking-the-input)
        - [Variable Expansion](#variable-expansion)
        - [Splitting the Command](#splitting-the-command)
        - [Creating the Heredocs](#creating-the-heredocs)
        - [Getting the Arguments](#getting-the-arguments)
        - [Checking for Single Builtin](#checking-for-single-builtin)
        - [Preparing the Redirections](#preparing-the-redirections)
    - [Executing](#executing)
        - [Creating the Pipes](#creating-the-pipes)
        - [Creating the Child](#creating-the-child)
        - [The Parent Process](#the-parent-process)
        - [The Child Process](#the-child-process)
- [Conclusion](#conclusion)
- [Known Issues](#known-issues)
- [Other](#other)

## How to use it

Requirements:
- Linux
- GNU make
- Clang to compile
- Readline

Install readline
```bash
sudo apt install libreadline-dev
```
Clone the repo :
```bash
git clone https://github.com/Nabil080/minishell.git
cd minishell
```
Compile and execute (or directly run `make test`)
```bash
make && ./minishell
```
Check for leaks with 
```bash
make leak
```
## What it does

Minishell allows you to execute any binary, either by giving its complete path or by using the "PATH" environment variable. 

It also has some built-ins like the "cd" or "echo" commands and support pipes (`|`) and redirection tokens (`<<`, `<`, `>`, `>>`).

Minishell prompts you for an input, execute it, and then prompts you again until you quit the program with an exit or a ctrl+D on an empty line

It also supports Ctrl+C signal and Ctrl+\ inside blocking commands like ```cat```
## How it does it

The project has two big parts: **Parsing** and **Executing**

But before that there are a few things to be done.

We first start by duplicating our environment so we can later change it, and by setting the handlers for our signals.

Once this is done, the program loop starts and we prompt the user for an input using the `readline()` function. This line will be used as our input for the rest of the program.

### The Parsing

The parsing is quite the challenge in this project, as such it had to be separated quite a bit.

#### **Checking the input**

We start by checking if the input is valid. An input is invalid when there is a syntax error near with our tokens:
- Redirections token should have a name after them
- Pipes shouldn't be empty
- Quotes shouldn't be open (this one is supported by bash with multi line inputs)

Since this is a minimalist recreation, we don't handle everything that bash does. Some tokens like the logical operators `&&` and `||` are treated as regular words.

#### **Variable expansion**

Let's suppose we accepted this input:

`
<<EOF cat > out'file' | echo "$USER is coding." | tr a-z A-Z >createme >> "out"file
`

We start by **expanding the variables**, which mean we check if they exist inside our environment, and if they do, we replace them by their value.

If they don't, we simply remove the variable.
If it's `$$` we replace it by a single `$` and if it's `$?` we replace it with the last exit code.

We end up with this input :

`
<<EOF cat > out'file' | echo "Nabil080 is > 'coding'." | tr a-z A-Z >createme >>          "out"file
`

*Note that we only expand the variables that are outside single quotes. `'$USER'` would stay the same.*

#### **Splitting the command**

Our line is actually made of 3 simple command, separated by the pipes. We will store each one in an array and treat them independently.

Each command will be a **structure** storing the line, the arguments to be executed, the infile to read from and the outfile to write to.

At first, only the line is set. The redirections and the arguments are defined after.

There we would have an array with these three commands:
- cmds[0]->line = `<<EOF cat > out'file'`
- cmds[1]->line = `echo "Nabil080 is > 'coding'."`
- cmds[1]->line = `tr a-z A-Z >createme >>          "out"file`

*Each command also has a boolean to know wether or not it should be executed. As soon as an error is found during the parsing, it is set to false.*

#### **Creating the heredocs**

The heredoc `<<` token doesn't behave like the others redirection.
This type of redirection instructs the shell to read input from the current source until a line containing the specified delimiter. All of the lines read up to that point are then used as the standard input

The heredoc input is stored in a temporary file, each command has its own based on its index `.heredoc_${index}.tmp`.

Since we don't want to be writing to multiple files at once, we want to create those files before forking and creating a child process per command.

Here only the first command has a heredoc `<<EOF cat > out'file'` 
We will prompt the user until we find the delimiter `EOF` or until a Ctrl+D. Then whatever was written will be stored inside `.heredoc_0.tmp`.

If we had multiple heredocs, we would replace this one or create the next one based on the command index.
The heredoc is now ready to be opened as infile during the redirection process.

cmds[0]->line is still equal to `<<EOF cat > out'file'` and cmds[0]->heredoc = true.

*if a SIGINT signal is sent during the heredoc creation, the current line is cancelled and we wait for a new prompt. As such, we had to modify the signal's handler during the heredocs creation.*

#### **Getting the arguments**

We then get the argument array for each command. This is needed for the `excve()` function which takes the command name and an array of arguments as parameters.

An argument is anything that isn't a redirection token or the word that follows one.
When quotes are opened, it is treated as a single argument and the tokens are ignored.
- cmds[0]->args[1] = `["cat"]`
- cmds[1]->args[2] = `["echo", "Nabil080 is > 'coding'."]`
- cmds[1]->args[3] = `["tr", "a-z", "A-Z"]`
As you can see, `"Nabil080 is > 'coding'."` is inside quotes and therefore treated as single argument. The redirection and the single quote are also ignored.

*It's easier to do this now since we have to check for the command name (which is the first argument) to know whether we are dealing with a builtin or not.*

#### **Checking for single builtin**

If we are dealing with a single command, and this command happens to be a builtin, we simply execute it and restart our program loop.
Otherwise, we call the `exec_cmds()` function which will execute each command inside a child process.

The builtins are `echo, cd, pwd, env, export, unset, exit`.

*We don't want our builtins to be executed inside a child process since it wont affect our main process and commands like exit or cd would be ineffective.*

#### **Preparing the redirections**

Here are our current cmds[0] :
- line: `<<EOF cat > out'file'`
- args: `["cat"]`
- is_valid: `true`
- is_heredoc: true
- in_fd: `0`
- out_fd: `0`

Our goal here is to prepare our file descriptors for our execution.

We will parse our string **left to right** until there is no redirection token left.
Each time we find one, we will try to update the corresponding file descriptor and remove the token and filename from the line.
If it's :
- An infile redirection `<`, we will try to open the filename; if we are successful, we store the fd inside cmds[0]->in_fd. Otherwise **we throw an error**.
- A heredoc redirection `<`, it's the same but we open `.heredoc_${index}.tmp`.
- An truncate redirection `>`, we try to open the file in truncate mode, if the file does not exist we create it and if it exists but we don't have the rights, **we throw an error**.
- An append redirection `>>`, it's the same but we open in append mode.

**As soon as an error is detected**, an error message is printed, is_valid is set to false and we stop our parsing

*Since we open our files from left to right, only the last one will be saved. However, the ones before will still be created in the case of an outfile. `cat >a >b` will create both a and b.*

### The executing

We now have everything we need to execute our commands. We know their arguments, their infile/outfile and if they are valid or not.

We can start looping through our commands.

#### **Creating the pipes**

We create a pipe for each command. The reason we do it before forking is because we need the pipe both in the child and the parent.

The write end of the pipe will be used for the **output of the current cmd**, while the read end will be used for the **input of the next cmd**

Knowing that we write to pipe[1] and we read from pipe[0], we will have something like this :
`cmd[0]->pipe[1]->pipe[0]->cmd[1]`. Then we close the pipe and start again with the next command.

#### **Creating the child**

Now that we have created our pipes, it start to create a child. We do so by duplicating the current process with the `fork()` function, resulting in a parent process being our main process, and a child being the process in which we will execute our commands.

#### **The parent process**

The parent process only need to prepare the input of the next cmd. As such, it will update `cmd[0 + 1]->in_fd` to be equal to a duplicate of `pipe[0]` **but only if its in_fd hasn't been set already**.

We only duplicate the pipe if there is no redirection. 

*Of course we don't forget to close our open file descriptors*

#### **The child process**

Since the child process already has the correct `in_fd`, which is either :
- A redirection detected during the parsing
- STDIN if it's the first command
- The read end of a pipe that was duplicated during the previous loop
We can safely substitute STDIN by `in_fd` using the `dup2()` function.
However, the `out_fd` hasn't been determined yet and it's up to the child to do so.

The output fd can either be : (sorted by priority)
- A redirection detected during the parsing
- STDOUT if it's the last command
- The write end of the current pipe
We determine in which case we are and this time we use `dup2()` to substitute STDOUT.

We once again don't forget to close our open fds, everything we need has already been duplicated to stdin/stdout. We can also safely close stdin since this is a subprocess.

*If `cmd[i]->is_valid` is set to false, we simply skip this whole process and exit the child*

Now that we have our redirections, all that's left is to execute the command depending on its name :
- **If it's a builtin**, we call our own function and exit the child ourself.
- **If it's a relative or absolute path**, we directly check for it's existence and it's execution rights.
- **If it's a simple name and the PATH environment variable is set**, we loop through the environment and we return the first valid command found.

If no command is valid, we print the corresponding error message and exit the child.
Otherwise, we call `execve()` with the correct binary.

*`Execve()` does not return but instead replaces the current process on success. If it did return we know that the execution failed and we do the corresponding treatment*

Finishing our exemple :
- The first command `cat > out'file'` will read from the heredoc and write to outfile.
- The second command `echo "$USER is > 'coding'."` will read from the output of the first command through a pipe, which will be empty since the last command wrote to an outfile.
- The third command `tr a-z A-Z >createme >> "out"file` will transform the output from the second command and append it to outfile. It also created "createme"

We end up with this output in the "outfile"
```
${Whatever was written in the heredoc during the first command}
NABIL080 IS > 'CODING'.
```

### Conclusion

That's the gist of it. Once the input has been correctly executed, we prompt the user again and redo the whole thing. 

Even though it's a minimalist recreation, there's still much more to this project. Minor things such as the error messages, the exit codes, the different signal handlers depending on the ongoing treatment, the single/double quotes handling, the various small features like `cd` using and updating the environment... which all adds up to days of work.

This project was a good learning experience because of how much you can improve it. You have to set your own boundaries and know when to let go.
## Known Issues

- `./minishell | ./minishell` isn't supported. Our shell only works in interactive mode. This is solved by checking if we are in a tty or not before showing our prompt.
- `Ctrl+\` does not print a new line.
- Export variable names are considered invalid when there is a non alphanumeric character in the middle. Things like `export TEST_TEST=test` will be detected as invalid syntax.
## Other

This project has been written according to the norm imposed at [@42School](https://github.com/42School). Therefore, it might look a bit strange for those not familiar with it.
