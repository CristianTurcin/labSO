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
char buffer[4096];

void writeInFile(char *buffer)
{
  if(write(fOut,buffer,strlen(buffer))==-1)
      {
        perror("write in file output error");
        exit(-1);
      }

}
char *getOtherPermissions(struct stat fileInfo)
{
  char *otherP="---";
  // Others permissions
    
     if(fileInfo.st_mode & S_IROTH)
        otherP[0]='r';

    if(fileInfo.st_mode & S_IWOTH)
        otherP[1]='w';

    if(fileInfo.st_mode & S_IXOTH)
        otherP[2]='x';
  return otherP;

}
char *getGroupPermissions(struct stat fileInfo)
{
  char *groupP="---";
  // Group permissions
    
    if(fileInfo.st_mode & S_IRGRP)
        groupP[0]='r';

    if(fileInfo.st_mode & S_IWGRP)
        groupP[1]='w';

    if(fileInfo.st_mode & S_IXGRP)
        groupP[2]='x';  
    return groupP;    

}
char *getOwnerPermissions(struct stat fileInfo)
{
  char *ownerP="---";

    // Owner permissions

    if(fileInfo.st_mode & S_IRUSR)
        ownerP[0]='r';

    if(fileInfo.st_mode & S_IWUSR)
        ownerP[1]='w';

    if(fileInfo.st_mode & S_IXUSR)
        ownerP[2]='x';

    
    return ownerP;
}
/*int openFile(const char *fileName)
{
  int fIn;
  if( ( fIn = open(fileName,O_RDONLY)) < 0 )
    {
      perror("input file error \n");
      exit(-1);
    }
  return fIn;
}*/

void generateBmpStats(struct dirent *info,struct stat fileInfo)
{
  /*int fIn=openFile(info->d_name);

  lseek(fIn,18,SEEK_CUR);
  int heigth=0,length=0;
  read(fIn,&heigth,sizeof(int));
  read(fIn,&length,sizeof(int));*/

  sprintf(buffer,
            "\nBMP File name: %s \n dimensiune: %lld bytes\n user id: %d\n contorul de legatur: %ld\n timpul ultimei modificari: %s drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces other: %s\n", 
            info->d_name,(long long)fileInfo.st_size,fileInfo.st_uid, fileInfo.st_nlink,ctime(&fileInfo.st_mtime),getOwnerPermissions(fileInfo),getGroupPermissions(fileInfo),getOtherPermissions(fileInfo));
  writeInFile(buffer);
  //close(fIn);

}
void generateRegFileStats(struct dirent *info,struct stat fileInfo)
{
  
  sprintf(buffer,
            "\nregular file name: %s \n dimensiune: %lld bytes\n user id: %d\n contorul de legatur: %ld\n timpul ultimei modificari: %s drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces other: %s\n", 
            info->d_name,(long long)fileInfo.st_size,fileInfo.st_uid, fileInfo.st_nlink,ctime(&fileInfo.st_mtime),getOwnerPermissions(fileInfo),getGroupPermissions(fileInfo),getOtherPermissions(fileInfo));
  writeInFile(buffer);

}
void generateDirStats(struct dirent *info,struct stat fileInfo)
{
  
  sprintf(buffer,
            "\nnume director: %s\n user id: %d\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces other: %s\n", 
            info->d_name,fileInfo.st_uid,getOwnerPermissions(fileInfo),getGroupPermissions(fileInfo),getOtherPermissions(fileInfo));
  writeInFile(buffer);

}
void generateLinkStats(struct dirent *info,struct stat fileInfo)
{
   sprintf(buffer,
            "\nNume legatura: %s \n dimensiune: %lld bytes\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces other: %s\n", 
            info->d_name,(long long)fileInfo.st_size,getOwnerPermissions(fileInfo),getGroupPermissions(fileInfo),getOtherPermissions(fileInfo));
  writeInFile(buffer); 

}

char *getFileExtension(char *fileName) {
    char *token;
    char *ext = NULL;
    const char dot[] = ".";

    token = strtok(fileName, dot); 
    
    while (token != NULL) {
        ext = token;
        token = strtok(NULL, dot);
    }

    return ext;
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
      
      if (strcmp(info->d_name, ".") == 0 || strcmp(info->d_name, "..") == 0) {
            continue;
      }
      

      if(S_ISREG(fileInfo.st_mode)==0)
      {
        if(info->d_type == DT_REG)
        {
          if(strcmp(getFileExtension(info->d_name),"bmp")==0)
          {
            generateBmpStats(info,fileInfo);

          }
          else
          {
          
            generateRegFileStats(info,fileInfo);

          }

          
        }
        else if(info->d_type == DT_DIR)
        {
          
          generateDirStats(info,fileInfo);

        }
        else if(info->d_type == DT_LNK)
        {
          generateLinkStats(info,fileInfo);
        }
      }
      
         
    }
    close(fOut);
    closedir(dir);
}

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
  verifyInput(argc,argv);

  createOutputFIle();

  workDir(argv[1]);

  
  return 0;
}