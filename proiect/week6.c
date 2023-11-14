#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>



int main(int argc, char *argv[])
{

    int fIn,fOut;
    char buffer[BUFSIZ];

    struct stat fileInfo;
    stat(argv[1], &fileInfo);

    //arg check
    if (argc != 2 || S_ISREG(fileInfo.st_mode) == 0)
    {
      perror("Usage ./program <fisier_intrare>\n");
      exit(-1);
    }

    if( (fIn = open(argv[1],O_RDONLY)) < 0 )
    {
      perror("input file error \n");
      exit(-1);
    }
   
    //photoDim
    lseek(fIn,18,SEEK_CUR);
    int heigth=0,length=0;
    read(fIn,&heigth,sizeof(int));
    read(fIn,&length,sizeof(int));

   //Permissions

    char ownerP[4]="---",groupP[4]="---",otherP[4]="---";

    // Owner permissions

    if(fileInfo.st_mode & S_IRUSR)
        ownerP[0]='r';

    if(fileInfo.st_mode & S_IWUSR)
        ownerP[1]='w';

    if(fileInfo.st_mode & S_IXUSR)
        ownerP[2]='x';

    // Group permissions
    
    if(fileInfo.st_mode & S_IRGRP)
        groupP[0]='r';

    if(fileInfo.st_mode & S_IWGRP)
        groupP[1]='w';

    if(fileInfo.st_mode & S_IXGRP)
        groupP[2]='x';

    // Others permissions
    
     if(fileInfo.st_mode & S_IROTH)
        otherP[0]='r';

    if(fileInfo.st_mode & S_IWOTH)
        otherP[1]='w';

    if(fileInfo.st_mode & S_IXOTH)
        otherP[2]='x';

    //write in file the info
    
    if( (fOut = open("statistica.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 )
    {
      perror("output file error \n");
      exit(-1);
    }

    sprintf(buffer,
            "File name: %s\n inaltime: %d\n lungime: %d\n dimensiune: %lld bytes\n user id: %d\n contorul de legatur: %ld\n timpul ultimei modificari: %s\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces other: %s\n", 
            argv[1],heigth,length,(long long)fileInfo.st_size,fileInfo.st_uid, fileInfo.st_nlink,ctime(&fileInfo.st_mtime),ownerP,groupP,otherP);

    if(write(fOut,buffer,strlen(buffer))==-1)
    {
      perror("write in file output error");
      exit(-1);
    }

    
    close(fIn);
    close(fOut);
    return 0;

}