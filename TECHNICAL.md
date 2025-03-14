# Minishell - Technical Details

## Overview

Minishell operates in two main phases: **Parsing** and **Execution**. It also handles signals, heredocs, and environment variables.

## 1. Initialization

Before entering the main loop, Minishell:
- Duplicates the environment for modifications.
- Sets up signal handlers.
- Prepares the prompt using `readline()`.

## 2. Parsing

The input command is broken down into structured data:

### **Example Input:**
```sh
echo "The user is $USER" | tr a-z A-Z | cat -e > output.txt
```

### **Syntax Checking**
- Ensures valid syntax for redirections (`<`, `>`, `>>`, `<<`) and pipes (`|`).
- Detects unmatched quotes.

### **Variable Expansion**
- Replaces `$USER` with its value from the environment (e.g., `Nabil080`).
- The command transforms into:
  ```sh
  echo "The user is Nabil080" | tr a-z A-Z | cat -e > output.txt
  ```

### **Command Splitting**
- Minishell splits the input into separate commands:
  ```sh
  cmds[0] = "echo \"The user is Nabil080\""
  cmds[1] = "tr a-z A-Z"
  cmds[2] = "cat -e > output.txt"
  ```

### **Heredoc Handling (`<<`)**
- If a heredoc is used (`<<EOF`), Minishell stores the input in a temporary file for later execution.

## 3. Execution

Commands are executed in separate processes, except for built-ins.

### **Pipes and Redirections**
- `|` connects `echo` to `tr`, passing the output.
- The second `|` sends the modified text to `cat -e`.
- `>` redirects `cat -e`'s output to `output.txt`.

### **Built-in Commands**
Built-ins (`cd`, `export`, `exit`, etc.) execute **inside the main shell process**.

### **Child Processes**
- `fork()` creates a new process for each external command.
- `execve()` runs external commands.
- The parent process waits and handles pipes.

### **Final Execution Flow:**
1. `echo "The user is Nabil080"` runs and outputs:
   ```sh
   The user is Nabil080
   ```
2. The output is passed to `tr a-z A-Z`, converting it to:
   ```sh
   THE USER IS NABIL080
   ```
3. The modified text is passed to `cat -e`, which appends `$` to indicate line endings:
   ```sh
   THE USER IS NABIL080$
   ```
4. `>` redirects the final output to `output.txt`. The contents of `output.txt` are:
   ```sh
   THE USER IS NABIL080$
   ```

## 4. Signal Handling

- `Ctrl+C` → Interrupts the current process.
- `Ctrl+D` → Exits the shell.
- `Ctrl+\` → Handles blocking commands like `cat`.

## Conclusion

Minishell recreates a Bash-like shell with simplified parsing and execution. While minimalist, it efficiently handles commands, pipes, and signals.

---

For installation and usage, check the [`README.md`](README.md).

