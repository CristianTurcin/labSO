#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>
#include <dirent.h>

char buffer[4096];


int countLines(char *fileName) {
    int fIn;
    if ((fIn = open(fileName,O_RDONLY)) < 0) {
        perror("Error opening file(countLines funct)");
        exit(EXIT_FAILURE);
    }

    int lines = 0;
    char ch;
    while (read(fIn, &ch, 1) > 0) {
        if (ch == '\n') {
            lines++;
        }
    }
    
    close(fIn);
    return lines;
}

void writeNoOfLinesInFile(char *fileName)
{
  int fOut,lines=0;
  char linesBuff[BUFSIZ];
  if( (fOut = open("statistica.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 )
    {
      perror("output file error \n");
      exit(-1);
    }
  lines=countLines(fileName);
  snprintf(linesBuff,sizeof(linesBuff),"s-au scris %d lini-%s",lines,fileName);
  if(write(fOut,linesBuff,strlen(linesBuff))==-1)
      {
        perror("writing nr of lines in file output error");
        exit(-1);
      }
  close(fOut);

}

void writeInFile(char *buffer, char *fileName, char *dirName)
{
    char outFileName[500] = "";
    char *p = strtok(fileName, ".");
    strcat(outFileName, p);
    strcat(outFileName, "_statistica");
    strcat(outFileName, ".txt");

    char pathName[500] = "./";
    strcat(pathName, dirName);
    strcat(pathName, "/");
    strcat(pathName, outFileName);

    int fp;

    if ((fp = open(pathName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) < 0) {
        perror("Error opening bmp header");
        exit(EXIT_FAILURE);
    }

    if(write(fp,buffer,strlen(buffer))==-1)
      {
        perror("write in file output error");
        exit(-1);
      }
    writeNoOfLinesInFile(pathName);
    

    close(fp);
}

int openFileInDirectory(char *dirName,char *fileName) {
    
    char fullPath[1024];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", dirName, fileName);

    
    int bmpFile;
    if ((bmpFile = open(fullPath, O_RDONLY)) < 0) {
        perror("Error opening file ");
        exit(EXIT_FAILURE);
    }

  return bmpFile;
}

void noOfCorrectProp(char *caracter)
{
  int pfd[2];
	pid_t pid;
  char aa[4096];
	
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
        close(pfd[0]);
        dup2(pfd[1], STDOUT_FILENO);
        close(pfd[1]);
        //execl("./script.sh","script.sh", caracter, NULL);
        exit(0);
	}else
    {
      close(pfd[1]); 
	    read(pfd[0],aa,sizeof(aa));
      printf("%s",aa);
	    close(pfd[0]);
      wait(NULL);
    }
    
   
}

void extractFileContent(char *dirName,struct dirent *info,struct stat fileInfo)
{
  int pfd[2];
	pid_t pid;
  char pipeBuff[4096];
  char aa[4096];

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
      close(pfd[0]);
      int fIn = openFileInDirectory(dirName,info->d_name);
      int readFile = read(fIn,pipeBuff,4096);
      printf("%d\n",readFile);//folosit doar sa scap de warningul: variable readFile set but not used
      write(pfd[1],pipeBuff,sizeof(pipeBuff));
	    close(pfd[1]); 
	    exit(0);
	}else
    {
      close(pfd[1]);
	    read(pfd[0],aa,sizeof(aa));
      printf("continutul fisierului %s este: %s\n",info->d_name,aa);
	    close(pfd[0]);
    }
	
}

void generateBmpStats(char *dirName,struct dirent *info,struct stat fileInfo,char *outDir)
{
  int fIn = openFileInDirectory(dirName,info->d_name);
  lseek(fIn,18,SEEK_CUR);
  int heigth=0,length=0;
  read(fIn,&heigth,sizeof(int));
  read(fIn,&length,sizeof(int));


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
  writeInFile(buffer,info->d_name,outDir);
  free(ownerP);
  free(groupP);
  free(otherP);
  

}

void generateRegFileStats(struct dirent *info,struct stat fileInfo,char *outDir)
{
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
  writeInFile(buffer,info->d_name,outDir);
  free(ownerP);
  free(groupP);
  free(otherP);
}

void generateLinkStats(struct dirent *info,struct stat fileInfo,char *outDir)
{
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
  writeInFile(buffer,info->d_name,outDir);
  free(ownerP);
  free(groupP);
  free(otherP);

}

void generateDirStats(struct dirent *info,struct stat fileInfo, char *outDir) {
   
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
    writeInFile(buffer,info->d_name,outDir); 

    free(ownerP);
    free(groupP);
    free(otherP);
}

void workDir(char *dirName,char *outDir,char *caracter) {
    DIR *dir; 
    struct dirent *info;
    dir = opendir(dirName);
    char fullpath[4096];
    pid_t pID;

    
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
  
    while ((info = readdir(dir)) != NULL) {
        
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirName, info->d_name);

        struct stat fileInfo;
        stat(fullpath, &fileInfo);

        if (strcmp(info->d_name, ".") == 0 || strcmp(info->d_name, "..") == 0) {
            continue;
        }
        if((pID = fork()) < 0 )
        {
          perror("child process error!");
          exit(-1);
        }

        if (S_ISREG(fileInfo.st_mode)) {
          if(pID == 0)
          {
           if (strstr(info->d_name, ".bmp") != NULL)
          {
            pid_t pID2;
            if((pID2 = fork()) < 0 )
            {
             perror("fork 2  error!");
             exit(-1);
            }
            if(pID2 == 0)
            {
              //convertToGray(info->d_name);
            }
            else{
              int status;
              wait(&status);
              if (WIFEXITED(status))
                printf("GREY: procesul cu pid-ul %d și codul %d pt %s\n",pID,status, info->d_name);
              else
                printf("Something went wrong for procesul cu pid-ul %d și codul %d” pt %s\n",pID,status, info->d_name);
            }

            generateBmpStats(dirName,info,fileInfo,outDir);
            exit(EXIT_SUCCESS);
          
            
          }
          else
          {
            extractFileContent(dirName,info,fileInfo);
            generateRegFileStats(info,fileInfo,outDir);
            pid_t pID3;
            if((pID3 = fork()) < 0 )
            {
             perror("fork 3  error!");
             exit(-1);
            }
            
            if(pID3 == 0)
            {
              noOfCorrectProp(caracter);
            }
            else{
              int status;
              wait(&status);
              if (WIFEXITED(status))
                printf("extractContent: procesul cu pid-ul %d și codul %d” pt %s\n",pID,status, info->d_name);
              else
                printf("Something went wrong for procesul cu pid-ul %d și codul %d” pt %s\n",pID,status, info->d_name);
            }

           
           exit(EXIT_SUCCESS);
          }
          }
        } else if (S_ISDIR(fileInfo.st_mode)) {
          if(pID ==  0)
           {
            generateDirStats(info,fileInfo,outDir);
            exit(EXIT_SUCCESS);
           }
        } else if (S_ISLNK(fileInfo.st_mode)) {
          if(pID == 0)
          {
            generateLinkStats(info,fileInfo,outDir);
            exit(EXIT_SUCCESS);
          }
        } else {
            continue;
        }
      int status;
      wait(&status);
      if (WIFEXITED(status))
        printf("S-a încheiat procesul cu pid-ul %d și codul %d” pt %s\n",pID,WEXITSTATUS(status), info->d_name);
      else
        printf("Something went wrong for procesul cu pid-ul %d și codul %d” pt %s\n",pID,status, info->d_name);

      
    }
   

   closedir(dir); 
}

void verifyInput(int argc, char *argv[])
{
  struct stat fileInfo1;
  stat(argv[1], &fileInfo1);

  struct stat fileInfo2;
  stat(argv[2], &fileInfo2);   

    
  if (argc != 4)
  {
    perror("Usage ./program <director_intrare> <C>\n");
    exit(-1);
  }
  if(!S_ISDIR(fileInfo1.st_mode))
  {
    perror("first argument is not a directory\n");
    exit(-1);
  }
  if(!S_ISDIR(fileInfo2.st_mode))
  {
    perror("Second argument is not a directory\n");
    exit(-1);
  }

  if(!isalnum(argv[3][0]))
  {
    perror("3rd argument is not alphanumeric\n");
    exit(-1);
  }


}

int main(int argc, char *argv[])
{
  verifyInput(argc,argv);

  workDir(argv[1],argv[2],argv[3]);
  
  return 0;
}