//proces parinte citeste de la tast 1 char si trimite copilului prin un pipe charul iar copilul citeste si afiseaza pe ecran

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <unistd.h>

char buff[2];
int pfd[2];
int pid;


void parinte()
{
    close(pfd[0]);
    char cuvant[2];
    scanf("%s",cuvant);
    write(pfd[1],&cuvant,sizeof(cuvant));
    close(pfd[1]);
}
void fiu()
{
    char cuvant[2];
    close(pfd[1]);
    read(pfd[0],&cuvant,sizeof(cuvant));
    printf("%s\n",cuvant);
    close(pfd[0]);
}
int main(void)
{
	

	if(pipe(pfd)<0)
	{
	  perror("Eroare la crearea pipe-ului\n");
	  exit(1);
	}
	
	if((pid=fork())<0)
	{
	  perror("Eroare la fork\n");
	  exit(1);
	}
	if(pid==0) 
	{
      parinte();
      exit(0);
	}

    fiu();
	
	
	return 0;
}