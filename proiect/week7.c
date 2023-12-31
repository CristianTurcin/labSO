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
#include <limits.h>

DIR *dir;
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


int openFileInDirectory(const char *dirName, const char *fileName) {
    
    char fullPath[1024];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", dirName, fileName);

    
    int bmpFile;
    if ((bmpFile = open(fullPath, O_RDONLY)) < 0) {
        perror("Error opening bmp header");
        exit(EXIT_FAILURE);
    }

  return bmpFile;
  close(bmpFile);
}

void generateBmpStats(const char *dirName,struct dirent *info)
{
  int fIn = openFileInDirectory(dirName,info->d_name);
  lseek(fIn,18,SEEK_CUR);
  int heigth=0,length=0;
  read(fIn,&heigth,sizeof(int));
  read(fIn,&length,sizeof(int));

  

  struct stat fileInfo;
  stat(info->d_name, &fileInfo);
  char *ownerP = (char *)malloc(4 * sizeof(char));
  char *groupP = (char *)malloc(4 * sizeof(char)); 
  char *otherP = (char *)malloc(4 * sizeof(char)); 
  ownerP[0] = (fileInfo.st_mode & S_IRUSR) ? 'r' : '-';
  ownerP[1] = (fileInfo.st_mode & S_IWUSR) ? 'w' : '-';
  ownerP[2] = (fileInfo.st_mode & S_IXUSR) ? 'x' : '-';
  ownerP[3] = '\0';

  groupP[0] = (fileInfo.st_mode & S_IRGRP) ? 'r' : '-';
  groupP[1] = (fileInfo.st_mode & S_IWGRP) ? 'w' : '-';
  groupP[2] = (fileInfo.st_mode & S_IXGRP) ? 'x' : '-';
  groupP[3] = '\0';

  otherP[0] = (fileInfo.st_mode & S_IROTH) ? 'r' : '-';
  otherP[1] = (fileInfo.st_mode & S_IWOTH) ? 'w' : '-';
  otherP[2] = (fileInfo.st_mode & S_IXOTH) ? 'x' : '-';
  otherP[3] = '\0';


  sprintf(buffer,
            "\nBMP File name: %s \n heigth: %d \n length %d \n dimensiune: %ld bytes\n user id: %d\n contorul de legatur: %ld\n timpul ultimei modificari: %s drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces other: %s\n", 
            info->d_name,heigth,length,fileInfo.st_size,fileInfo.st_uid, fileInfo.st_nlink,ctime(&fileInfo.st_mtime),ownerP,groupP,otherP);
  writeInFile(buffer);
  free(ownerP);
  free(groupP);
  free(otherP);
  

}
void generateRegFileStats(struct dirent *info)
{
  struct stat fileInfo;
  stat(info->d_name, &fileInfo);
  char *ownerP = (char *)malloc(4 * sizeof(char));
    char *groupP = (char *)malloc(4 * sizeof(char)); 
    char *otherP = (char *)malloc(4 * sizeof(char)); 

    ownerP[0] = (fileInfo.st_mode & S_IRUSR) ? 'r' : '-';
    ownerP[1] = (fileInfo.st_mode & S_IWUSR) ? 'w' : '-';
    ownerP[2] = (fileInfo.st_mode & S_IXUSR) ? 'x' : '-';
    ownerP[3] = '\0';

    groupP[0] = (fileInfo.st_mode & S_IRGRP) ? 'r' : '-';
    groupP[1] = (fileInfo.st_mode & S_IWGRP) ? 'w' : '-';
    groupP[2] = (fileInfo.st_mode & S_IXGRP) ? 'x' : '-';
    groupP[3] = '\0';

    otherP[0] = (fileInfo.st_mode & S_IROTH) ? 'r' : '-';
    otherP[1] = (fileInfo.st_mode & S_IWOTH) ? 'w' : '-';
    otherP[2] = (fileInfo.st_mode & S_IXOTH) ? 'x' : '-';
    otherP[3] = '\0';

  sprintf(buffer,
            "\nregular file name: %s \n dimensiune: %ld bytes\n user id: %d\n contorul de legatur: %ld\n timpul ultimei modificari: %s drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces other: %s\n", 
            info->d_name,fileInfo.st_size,fileInfo.st_uid, fileInfo.st_nlink,ctime(&fileInfo.st_mtime),ownerP,groupP,otherP);
  writeInFile(buffer);
  free(ownerP);
  free(groupP);
  free(otherP);
}

void generateLinkStats(struct dirent *info)
{
  struct stat fileInfo;
  fstat(*info->d_name, &fileInfo);
    
    char *ownerP = (char *)malloc(4 * sizeof(char));
    char *groupP = (char *)malloc(4 * sizeof(char)); 
    char *otherP = (char *)malloc(4 * sizeof(char)); 

    ownerP[0] = (fileInfo.st_mode & S_IRUSR) ? 'r' : '-';
    ownerP[1] = (fileInfo.st_mode & S_IWUSR) ? 'w' : '-';
    ownerP[2] = (fileInfo.st_mode & S_IXUSR) ? 'x' : '-';
    ownerP[3] = '\0';

    groupP[0] = (fileInfo.st_mode & S_IRGRP) ? 'r' : '-';
    groupP[1] = (fileInfo.st_mode & S_IWGRP) ? 'w' : '-';
    groupP[2] = (fileInfo.st_mode & S_IXGRP) ? 'x' : '-';
    groupP[3] = '\0';

    otherP[0] = (fileInfo.st_mode & S_IROTH) ? 'r' : '-';
    otherP[1] = (fileInfo.st_mode & S_IWOTH) ? 'w' : '-';
    otherP[2] = (fileInfo.st_mode & S_IXOTH) ? 'x' : '-';
    otherP[3] = '\0';

  snprintf(buffer,sizeof(buffer),
            "\nNume legatura: %s \n dimensiune: %ld bytes\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces other: %s\n", 
            info->d_name,fileInfo.st_size,ownerP,groupP,otherP);
  writeInFile(buffer); 
  free(ownerP);
  free(groupP);
  free(otherP);

}



void generateDirStats(struct dirent *info) {
    struct stat fileInfo;
    fstat(*info->d_name, &fileInfo);

    char *ownerP = (char *)malloc(4 * sizeof(char));
    char *groupP = (char *)malloc(4 * sizeof(char)); 
    char *otherP = (char *)malloc(4 * sizeof(char)); 

    ownerP[0] = (fileInfo.st_mode & S_IRUSR) ? 'r' : '-';
    ownerP[1] = (fileInfo.st_mode & S_IWUSR) ? 'w' : '-';
    ownerP[2] = (fileInfo.st_mode & S_IXUSR) ? 'x' : '-';
    ownerP[3] = '\0';

    groupP[0] = (fileInfo.st_mode & S_IRGRP) ? 'r' : '-';
    groupP[1] = (fileInfo.st_mode & S_IWGRP) ? 'w' : '-';
    groupP[2] = (fileInfo.st_mode & S_IXGRP) ? 'x' : '-';
    groupP[3] = '\0';

    otherP[0] = (fileInfo.st_mode & S_IROTH) ? 'r' : '-';
    otherP[1] = (fileInfo.st_mode & S_IWOTH) ? 'w' : '-';
    otherP[2] = (fileInfo.st_mode & S_IXOTH) ? 'x' : '-';
    otherP[3] = '\0';

    snprintf(buffer,sizeof(buffer), 
    "\nnume director: %s\nidentificatorul utilizatorului: %d\ndrepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces other: %s\n\n", 
    info->d_name,fileInfo.st_uid, ownerP,groupP,otherP);
    writeInFile(buffer); 

    free(ownerP);
    free(groupP);
    free(otherP);
}


void workDir(const char *dirName) {
    struct dirent *info;
    dir = opendir(dirName);

    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((info = readdir(dir)) != NULL) {
        char fullpath[4096];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirName, info->d_name);

        struct stat fileInfo;
        stat(fullpath, &fileInfo);

        if (strcmp(info->d_name, ".") == 0 || strcmp(info->d_name, "..") == 0) {
            continue;
        }

        if (S_ISREG(fileInfo.st_mode)) {
           if (strstr(info->d_name, ".bmp") != NULL)
          {
            generateBmpStats(dirName,info);
            printf("bmp file %s \n",info->d_name);
          }
          else
          {
            printf("reg file %s \n",info->d_name);
            generateRegFileStats(info);

          }
        } else if (S_ISDIR(fileInfo.st_mode)) {
            generateDirStats(info);
            printf("dir file %s \n",info->d_name);
        } else if (S_ISLNK(fileInfo.st_mode)) {
            printf("link file %s \n",info->d_name);
            generateLinkStats(info);
        } else {
            continue;
        }
    }
   

    
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

  closedir(dir);
  close(fOut);



 
 
  
  return 0;
}