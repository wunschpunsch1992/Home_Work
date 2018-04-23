#include "launch.h"
#include "pipeline.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[])
{

  if(is_user_root())
  {
     arguments_comand_line* arguments = get_arguments(argc, argv);

    if(is_user_exist(arguments->user_id))
    {
        pid_t pid;
        int status;

        init_pipeline();
        parent_pipeline();

        printf("Parent uid=%d\n", getuid());

        pid = create_process();

        if(pid == 0)
        {
           children_pipeline();

           execute_program(arguments);
        }

        wait_process(pid, &status);
    }

    free(arguments);
  }
  
  return 0;
}

