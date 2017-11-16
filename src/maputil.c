#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "error.h"

int main(int argc, char * argv[]){
  int fd=open(argv[1],O_RDONLY,0640);
  if(fd==-1){
    exit_with_error("erreur ouverture");
  }
  if(strcomp(argv[2],"--getwidth")==0){
    int width;
    if(read(fd,&width,sizeof(int))==-1){
      exit_with_error("erreur lecture width");
    }
    printf("width : %d",width);
  }
  else if(strcomp(argv[2],"--getheight")==0){
    int width;
    if(read(fd,&width,sizeof(int))==-1){
      exit_with_error("erreur lecture width");
    }
  }
  else if(strcomp(argv[2],"--getobjects")==0){
    int width;
    if(read(fd,&width,sizeof(int))==-1){
      exit_with_error("erreur lecture width");
    }
  }
  else if(strcomp(argv[2],"-getinfo")==0){
    int width;
    if(read(fd,&width,sizeof(int))==-1){
      exit_with_error("erreur lecture width");
    }
  }
  return 0;
}
