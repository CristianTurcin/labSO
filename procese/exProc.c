#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int pid;
    char litera[10];

  
    if( ( pid=fork() ) < 0)
  {
    perror("Eroare");
    exit(1);
  }
if(pid==0)
  {
  
    execl("./student","studenti",argv[1],NULL);
    exit(0); 
  }
    return 0;
}