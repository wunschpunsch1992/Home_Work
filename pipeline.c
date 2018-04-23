#include  <unistd.h>
#include "pipeline.h"

static int fd1[2];
static int fd2[2];


void init_pipeline()
{
     pipe(fd1);
     pipe(fd2);

}

void children_pipeline()
{
   close(fd1[0]);
   dup2(fd1[1], 1);
   close(fd1[1]);

   close(fd2[0]);
   dup2(fd2[1], 2);
   close(fd2[1]);
}

void parent_pipeline()
{
    close(fd1[1]);
    dup2(fd1[0], 0);
    close(fd1[0]);

    close(fd2[1]);
    dup2(fd2[0], 0);
    close(fd2[0]);

}