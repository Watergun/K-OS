#ifndef PROCESS_H
#define PROCESS_H

//All implemented functions -> Description in .c file
int system_new_process(int process_code, char security_status, int unitsize);
int system_expand_process_memory(int* base, int unitsize); 
int system_start_process(int process, int argc, char **argv);
int system_name_process(int process, char* name);

void system_set_process_inbox(int process, int inbox);
void system_pit_service_routine();
void system_flash_memory_(int arg);
void system_process_manager_init();
void system_send_process_signal(int process, int signal);
void system_remove_process_signal(int process, int signal);




#endif
