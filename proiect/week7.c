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

int fOut;

void writeInFile(char *buffer)
{
  if(write(fOut,buffer,strlen(buffer))==-1)
      {
        perror("write in file output error");
        exit(-1);
      }

}

void workDir(const char *dirName) {
    DIR *dir;
    struct dirent *info;

    dir = opendir(dirName);

    
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    

    while ((info = readdir(dir)) != NULL) {
      
      struct stat fileInfo;
      stat(info->d_name, &fileInfo);  
      char buffer[4096];

      if(S_ISREG(fileInfo.st_mode)==0)
      {
        if(info->d_type == DT_REG)
        {

          sprintf(buffer,"reg file: %s\n",info->d_name);
          writeInFile(buffer);
        }
        else if(info->d_type == DT_DIR)
        {
          sprintf(buffer,"dir: %s\n",info->d_name);
          writeInFile(buffer);

        }
        else if(info->d_type == DT_LNK)
        {
          sprintf(buffer,"link: %s\n",info->d_name);
          writeInFile(buffer);
        }
        else
        {
          return;
        }
      }
      
         
    }
    close(fOut);
    closedir(dir);
}

/*
if (strcmp(info->d_name, ".") == 0 || strcmp(info->d_name, "..") == 0) {
            continue;
        }
*/

void verifyInput(int argc, char *argv[])
{
    struct stat fileInfo;
    stat(argv[1], &fileInfo);    

    
    if (argc != 2 || S_ISDIR(fileInfo.st_mode) == 0)
    {
      perror("Usage ./program <director_intrare>\n");
      exit(-1);
    }

}
void createOutputFIle()
{
  if( (fOut = open("statistica.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 )
    {
      perror("output file error \n");
      exit(-1);
    }
}
int main(int argc, char *argv[])
{
  createOutputFIle();

  workDir(argv[1]);

  
  return 0;
}