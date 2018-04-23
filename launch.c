#include "launch.h"

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <pwd.h>
#include <getopt.h>

static void linux_error(const  char* x);
static void process_exit(int* status);
static arguments_comand_line* parse_arguments(int argc, char *argv[]);

pid_t create_process()
{
    pid_t pid;

    if((pid = fork()) < 0)
        linux_error("create_process error");

    return pid;
}


static void linux_error(const char* x) 
{ 
    perror(x); 
    exit(1); 

}

void wait_process(pid_t pid, int* status){

    if(wait(status) != pid)
        linux_error("wait_process error");

    process_exit(status);
}

static void process_exit(int* status){

    if(WIFEXITED(*status))
        printf("Normal completion. Exit code=%d\n", WEXITSTATUS(*status));

    else if(WIFSIGNALED(*status))
            printf("Abnormal termination. Signal number = %d%s\n", WTERMSIG(*status),
                #ifdef WCOREDUMP
                        WCOREDUMP(*status) ? "Create core dump" : "");
                #else
                        "");
                #endif
    else if(WIFSTOPPED(*status))
            printf("Child process stopped. Signal number = %d\n", WSTOPSIG(*status));

}

int is_user_root()
{
    uid_t user_id = getuid();

    return (user_id == 0);
}

int is_user_exist(uid_t user_id)
{
    struct passwd *user_info;
    user_info = getpwuid(user_id);

    return (user_info != NULL);

}

arguments_comand_line* get_arguments(int argc, char *argv[])
{
    
    if(argc > 3)
       return parse_arguments(argc, argv);
    else
        linux_error("The program must have: --uid -p");
    
}

/*
    функция "разбирает" командую строку и возвращает объект следующего вида:
        - user_id:      уникальный идентификатор пользователя
        - program_name: наименование програаммы с аргументами

         program_name имеет следующую структуру:
            -
*/
static arguments_comand_line* parse_arguments(int argc, char *argv[]){
    
    
	const char* short_options = "u:p:";

	const struct option long_options[] = {
		{"uid",     required_argument, NULL, 'u'},
		{"program", required_argument, NULL, 'p'},
		{NULL,0,NULL,0}
	};

	int rez;
	// opterr отвечает за вывод ошибок в stderr.
	// если opterr == 0 функция getopt_long не генерирует ошибку
	opterr = 0;

	arguments_comand_line* arguments = malloc(sizeof(arguments_comand_line));
	arguments->program_name =  malloc(sizeof(char*) * argc);
	int arguments_index = 1;
    int size_program_name = 0;


	
	while ((rez = getopt_long(argc, argv,   short_options,
						 	  long_options, NULL)) !=  -1)
	{

		switch(rez)
		{
			case 'u': 
			{
				if(optarg != NULL)
                {
                    if(atoi(optarg))
                    {
                        arguments->user_id = atoi(optarg);
                    }
                    else
                        linux_error("Bad uid");
                }
					
				break;
			};

			case 'p':
			{
				if(optarg != NULL)
				{
					arguments->program_name[0] = malloc(sizeof(char*));
				    arguments->program_name[0] = optarg;
                    size_program_name += 1;
				}
				break;
			}
			
			case '?': default: 
			{
				arguments->program_name[arguments_index] = malloc(sizeof(char*));
				//optind содержит индекс следующего символа в argv, который будет обрабатывать  getopt_long
				arguments->program_name[arguments_index] = argv[optind - 1];
				arguments_index += 1;
                size_program_name += 1;

				break;
			};
			
		}
			
	}
    
    //формируем терминирующие условие
    arguments->program_name[size_program_name] = malloc(sizeof(char*));
	arguments->program_name[size_program_name] = NULL;

    return arguments;
}

void execute_program(arguments_comand_line* arguments)
{
    if(arguments != NULL)
    {
         setuid(arguments->user_id);
         printf("Child uid=%d\n", getuid()); 

         if( arguments->program_name  && arguments->program_name[0])
             execvp(arguments->program_name[0],  arguments->program_name);
            
         exit(1);
    }
}





