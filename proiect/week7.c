#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>

void opendDir(const char *path) {
    DIR *dir;
    struct dirent *entry;

    // Deschide directorul
    dir = opendir(path);

    // Verifică dacă directorul s-a deschis cu succes
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Parcurge fiecare intrare din director
    while ((entry = readdir(dir)) != NULL) {
        /*
            if(bmp)
            {

            }
            else if(reg file)
            {

            }
            else if(simbolic link)
            {
                nume legatura: nume
                dimensiune legatura: dimensiunea legaturii
                dimensiune fisier dimensiunea fisierului target
                drepturi de acces user legatura: RWX
                drepturi de acces grup legatura: R–-
                drepturi de acces altii legatura: ---   
            }
            else if(dir)
            {
                nume director: director
                identificatorul utilizatorului: <user id>
                drepturi de acces user: RWX
                drepturi de acces grup: R–-
                drepturi de acces altii: ---
            }
            else()
            {
                exit
            }
        */
    }

    closedir(dir);
}


int main(int argc, char *argv[])
{
    int fIn,fOut;
    char buffer[BUFSIZ];

    struct stat fileInfo;
    stat(argv[1], &fileInfo);

    
    if (argc != 2 || S_ISDIR(fileInfo.st_mode) == 0)
    {
      perror("Usage ./program <director_intrare>\n");
      exit(-1);
    }


    opendDir(argv[1]);

    if( (fOut = open("statistica.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 )
    {
      perror("output file error \n");
      exit(-1);
    }

    close(fIn);
    close(fOut);
    return 0;
}