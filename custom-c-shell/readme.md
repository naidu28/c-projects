# Writing a custom shell in C

Following tutorial at this link: https://brennan.io/2015/01/16/write-a-shell-in-c/


## Lifetime of a shell:

* Initialize - shell reads and executes its config files
* Interpret - shell reads commands from stdin (interactive gui or file), and executes them
* Terminate - after its commands are executed, the shell executes any shutdown commands, frees up any memory, and terminates

