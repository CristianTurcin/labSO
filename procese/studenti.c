#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    if(strcmp(argv[1],"a") == 0)
    {
        printf("TURCIN");
    }
    else
    {
        printf("SUBIN");
    }
    
    return 0;
}