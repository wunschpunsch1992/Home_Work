#ifndef LAUNCH_H_
#define LAUNCH_H_

#include<sys/types.h>


pid_t create_process();

void wait_process(pid_t pid, int* status);

int is_user_exist(uid_t user_id );

int is_user_root();


typedef struct arguments_comand_line{
    int user_id;
   char** program_name;

} arguments_comand_line;

void execute_program(arguments_comand_line* arguments);

arguments_comand_line* get_arguments(int argc, char *argv[]);

#endif
