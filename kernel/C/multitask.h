#ifndef MULTITASK_H
#define MULTITASK_H

//All implemented functions -> See the .c file for descriptions
void ph_process_end_loop();
void ph_add_process(int* process);

int ph_switch_process(int previous_stackmark);

//Multitask controlling values
int *global_system_pit;
int __process__;

//"do not switch processes" -> When > 0: deactivates MT until set to 0 again
int global_critical_section;

#endif
