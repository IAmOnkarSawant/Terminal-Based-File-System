# Assignment No 2 
## Question 1
### Overview
- Implement a shell that supports a semi-colon separated list of commands. Use 'strtok' to
tokenize the command. Also, support '&' operator which lets a program run in the background
after printing the process id of the newly created process. Write this code in a modular fashion.
-The goal of the assignment is to create a user-defined interactive shell program using
c/cpp that can create and manage new processes. The shell should be able to create a
process out of a system program like emacs, vi, or any user-defined executable. Your
shell can handle background and foreground processes and also input/output
redirections and pipes.
-The following are the specifications for the assignment. For each of the requirements, an
appropriate example is given along with it:

### Execution
- By executing following commands in the terminal (LINUX) you can run the program.

```shell
make -f Terminal.mk

./Terminal
```

### Functions Included
- following functions are implementaed in this application
    1) Prompt
    2) cd
    3) ls
    4) pwd
    5) pinfo
    6) search
    7) redirection
    8) signal
    9) autocomplete
    10) foreground and background
    11) echo




### Assumption
- I have assumed that program in which it is invoked is HOME 
- In auto complete please enter after pressing the tab
***

