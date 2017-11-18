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
    perror("erreur ouverture");
  }
  
  if(strcmp(argv[2],"--getwidth")==0){
    int width;
    if(read(fd,&width,sizeof(int))==-1){
      perror("erreur lecture width");
    }
    printf("width : %d \n",width);
  }
  
  else if(strcmp(argv[2],"--getheight")==0){
    int height;
    lseek(fd,sizeof(int),SEEK_SET);
    if(read(fd,&height,sizeof(int))==-1){
      perror("erreur lecture height");
    }
    printf("height : %d \n",height);
  }
  
  else if(strcmp(argv[2],"--getobjects")==0){
    int nb;
    lseek(fd,2*sizeof(int),SEEK_SET);
    if(read(fd,&nb,sizeof(int))==-1){
      perror("erreur lecture nombre d'objets");
    }
    printf("nombre d'objets : %d \n", nb);
  }
  
  else if(strcmp(argv[2],"-getinfo")==0){
    int buf;
    if(read(fd,&buf,sizeof(int))==-1){
      perror("erreur lecture width");
    }
    printf("width : %d \n", buf);
    
    if(read(fd,&buf,sizeof(int))==-1){
      perror("erreur lecture height");
    }
    printf("height : %d \n", buf);
    
    if(read(fd,&buf,sizeof(int))==-1){
      perror("erreur lecture nombre d'objets");
    }
    printf("nombre d'objets : %d \n", buf);
  }
  return 0;
}
