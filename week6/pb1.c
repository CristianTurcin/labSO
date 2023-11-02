#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
  
  int fIn,fOut,rd,countLower=0,countUpper=0,countDigit=0,countAlpha=0,countCh=0;
  char buffer[BUFSIZ];
  char buffer2[BUFSIZ];
  char ch = argv[3][0];
  if (argc != 4)
    {
      perror("not enough params\n");
      exit(-1);
    }

  if( (fIn = open(argv[1],O_RDONLY)) < 0 )
    {
      perror("input file error \n");
    }

  if( (fOut = open(argv[2],O_WRONLY)) < 0 )
    {
      perror("output file error \n");
    }
 
  rd = read(fIn, buffer, BUFSIZ);
  for (int i=0; i<rd; i++)
    {
      if(isupper(buffer[i]))
	countUpper++;
      else if(islower(buffer[i]))
	countLower++;
      else if(isdigit(buffer[i]))
	countDigit++;
      else if(isalpha(buffer[i]))
	countAlpha++;
      if(ch == buffer[i])
	countCh++;
	     
      
    }
 
  int fileSize=0;

  struct stat fileInfo;
  fileSize = fstat(fIn, &fileInfo);
  

  sprintf(buffer2,"upper case: %d\n lower case: %d\n digits: %d\n char: %d\n file size: %d\n ",countUpper,countLower,countDigit,countCh,fileSize);

  if(write(fOut,buffer2,strlen(buffer2))==-1)
    {
      perror("write in file output error");
    }

  //info print just for test
  printf("upper case: %d\n",countUpper);
  printf("lower case: %d\n",countLower);
  printf("digits: %d\n",countDigit);
  printf("char: %d\n",countCh);

  printf("file size: %ld\n", fileInfo.st_size);
  close(fIn);
  close(fOut);


  return 0;
  
}
