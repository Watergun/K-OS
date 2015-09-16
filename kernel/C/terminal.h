#ifndef TERMINAL_H
#define TERMINAL_H

//Declarate all implemented terminal functions
int terminal(int argc, char **argv);
int  terminal_help(int argc, char **argv);

void terminal_set_statement();
void terminal_print_string(char *str);
void terminal_exec_line();
void terminal_pass_char(char c);
void terminal_swap_buffers();
void terminal_start_process(int code, int argc, char **argv);
void terminal_pass_char(char c);

char **terminal_split_exec_line(char *buf, int *pargs);

#endif
