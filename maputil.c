#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../include/error.h"
#include "../include/map.h"

int main(int argc, char * argv[]){
  int fd=open(argv[1],O_RDWR,0640);
  if(fd==-1){
    exit_with_error("erreur ouverture fd\n");
  }
  int new=open("new.map",O_WRONLY | O_CREAT | O_TRUNC,0640);
  if(new==-1){
    exit_with_error("erreur ouverture\n");
  }
  int width;
  int height;
  int nb;
  int buf;
  
  
  if(strcmp(argv[2],"--getwidth")==0){
    if(read(fd,&width,sizeof(int))==-1){
      exit_with_error("erreur lecture width\n");
    }
    printf("width : %d \n",width);
  }
  
  else if(strcmp(argv[2],"--getheight")==0){
    lseek(fd,sizeof(int),SEEK_SET);
    if(read(fd,&height,sizeof(int))==-1){
      exit_with_error("erreur lecture height\n");
    }
    printf("height : %d \n",height);
  }
  else if(strcmp(argv[2],"--getobjects")==0){
    lseek(fd,2*sizeof(int),SEEK_SET);
    if(read(fd,&nb,sizeof(int))==-1){
      exit_with_error("erreur lecture nombre d'objets\n");
    }
    printf("nombre d'objets : %d \n", nb);
  }
  
  else if(strcmp(argv[2],"--getinfo")==0){
    if(read(fd,&buf,sizeof(int))==-1){
      exit_with_error("erreur lecture width\n");
    }
    printf("width : %d \n", buf);
    
    if(read(fd,&buf,sizeof(int))==-1){
      exit_with_error("erreur lecture height\n");
    }
    printf("height : %d \n", buf);
    
    if(read(fd,&buf,sizeof(int))==-1){
      exit_with_error("erreur lecture nombre d'objets\n");
    }
    printf("nombre d'objets : %d \n", buf);
  }
  
  
  
  else {
  	if(strcmp(argv[2],"--setwidth")==0){
  	
  	int w=atoi(argv[3]); //new width
    if(read(fd,&width,sizeof(int))==-1){
      exit_with_error("erreur lecture width\n");
    }
    if(write(new,&w,sizeof(int))!=sizeof(int)){
       exit_with_error("erreur écriture nouvelle largeur\n");
    }
    
    if(read(fd,&height,sizeof(int))==-1){ 
      exit_with_error("erreur lecture height\n");
    }
    if(write(new,&height,sizeof(int))!=sizeof(int)){
       exit_with_error("erreur écriture height\n");
    }
    
    if(read(fd,&nb,sizeof(int))==-1){
      exit_with_error("erreur lecture nb objets\n");
    }
    if(write(new,&nb,sizeof(int))!=sizeof(int)){
      exit_with_error("erreur écriture nb objets\n");
    }
    
    for(int i=0;i<nb;i++){
    	int taille;
    	if(read(fd,&taille,sizeof(int))==-1){
      exit_with_error("erreur lecture %d\n",i);
    	}
    	if(write(new,&taille,sizeof(int))!=sizeof(int)){
      exit_with_error("erreur écriture %d\n",i);
    	}
    	char c;
    	for(int j=0; j<taille; j++){
      	if(read(fd, &c, sizeof(char)) == -1){
        	exit_with_error("erreur lecture nom %d",i);
      	}
      	if(write(new,&c,sizeof(char))!=sizeof(char)){
      		exit_with_error("erreur écriture nom %d\n",i);
    		}
    	}
    	if(read(fd,&buf,sizeof(int))==-1){
      exit_with_error("erreur lecture frame %d\n",i);
    	}
    	if(write(new,&buf,sizeof(int))!=sizeof(int)){
      exit_with_error("erreur écriture frame %d\n",i);
    	}
    	if(read(fd,&buf,sizeof(int))==-1){
      exit_with_error("erreur lecture prop %d\n",i);
    	}
    	if(write(new,&buf,sizeof(int))!=sizeof(int)){
      exit_with_error("erreur écriture prop %d\n",i);
    	}    	   
    }
    if(w<width){
    	for(int i=0;i<height;i++){
    		for(int j=0;j<w;j++){
      		if(read(fd,&buf,sizeof(int))==-1){
        		exit_with_error("erreur lecture contenu\n");
      		}
      		if(write(new,&buf,sizeof(int))!=sizeof(int)){
      			exit_with_error("erreur écriture contenu \n");
    			} 
    		}
    		lseek(fd,(width-w)*sizeof(int),SEEK_CUR);
  		}
    }
    else{
    	for(int i=0;i<height-1;i++){
    		for(int j=0;j<width;j++){
      		if(read(fd,&buf,sizeof(int))==-1){
        		exit_with_error("erreur lecture contenu\n");
      		}
      		if(write(new,&buf,sizeof(int))!=sizeof(int)){
      			exit_with_error("erreur écriture contenu \n");
    			} 
    		}
    		for(int k=width;k<w;k++){
    			buf=-1;
    			if(write(new,&buf,sizeof(int))!=sizeof(int)){
      			exit_with_error("erreur écriture contenu supp \n");
    			}
  			}
    	}
    	for(int k=width;k<w;k++){ //on met un sol sur la nouvelle largeur
    		buf=0;
    		if(write(new,&buf,sizeof(int))!=sizeof(int)){
      		exit_with_error("erreur écriture nouveau sol \n");
    		}
    	}
    }    
  }
  	
  
  else if(strcmp(argv[2],"--setheight")==0){	
  	int h=atoi(argv[3]); //new height
    if(read(fd,&width,sizeof(int))==-1){
      exit_with_error("erreur lecture width\n");
    }
    if(write(new,&width,sizeof(int))!=sizeof(int)){
       exit_with_error("erreur écriture width\n");
    }
    
    if(read(fd,&height,sizeof(int))==-1){ 
      exit_with_error("erreur lecture height\n");
    }
    if(write(new,&h,sizeof(int))!=sizeof(int)){
       exit_with_error("erreur écriture nouvelle hauteur\n");
    }
    
    if(read(fd,&nb,sizeof(int))==-1){
      exit_with_error("erreur lecture nb objets\n");
    }
    if(write(new,&nb,sizeof(int))!=sizeof(int)){
      exit_with_error("erreur écriture nb objets\n");
    }
    
    for(int i=0;i<nb;i++){
    	int taille;
    	if(read(fd,&taille,sizeof(int))==-1){
      exit_with_error("erreur lecture %d\n",i);
    	}
    	if(write(new,&taille,sizeof(int))!=sizeof(int)){
      exit_with_error("erreur écriture %d\n",i);
    	}
    	char c;
    	for(int j=0; j<taille; j++){
      	if(read(fd, &c, sizeof(char)) == -1){
        	exit_with_error("erreur lecture nom %d",i);
      	}
      	if(write(new,&c,sizeof(char))!=sizeof(char)){
      		exit_with_error("erreur écriture nom %d\n",i);
    		}
    	}
    	if(read(fd,&buf,sizeof(int))==-1){
      exit_with_error("erreur lecture frame %d\n",i);
    	}
    	if(write(new,&buf,sizeof(int))!=sizeof(int)){
      exit_with_error("erreur écriture frame %d\n",i);
    	}
    	if(read(fd,&buf,sizeof(int))==-1){
      exit_with_error("erreur lecture prop %d\n",i);
    	}
    	if(write(new,&buf,sizeof(int))!=sizeof(int)){
      exit_with_error("erreur écriture prop %d\n",i);
    	}    	   
    }
    if(h<height){
    	lseek(fd,(height-h)*width*sizeof(int),SEEK_CUR);
    	for(int i=0;i<h;i++){
    		for(int j=0;j<width;j++){
      		if(read(fd,&buf,sizeof(int))==-1){
        		exit_with_error("erreur lecture contenu\n");
      		}
      		if(write(new,&buf,sizeof(int))!=sizeof(int)){
      			exit_with_error("erreur écriture contenu \n");
    			} 
    		}
  		}
    }
		  else{
		  	for(int i=0;i<h-height;i++){
		  		buf=1;
		  		if(write(new,&buf,sizeof(int))!=sizeof(int)){ //mur de gauche
		    			exit_with_error("erreur écriture contenu supp \n");
		  		}
		  		for(int j=1;j<width;j++){
		  			buf=-1;
		  			if(write(new,&buf,sizeof(int))!=sizeof(int)){
		    			exit_with_error("erreur écriture contenu supp \n");
		  			}
		  		}
		  	}
		  	for(int k=0;k<height;k++){
		  		for(int j=0;j<width;j++){
		  			if(read(fd,&buf,sizeof(int))==-1){
		      		exit_with_error("erreur lecture contenu\n");
		    		}
		    		if(write(new,&buf,sizeof(int))!=sizeof(int)){
		    			exit_with_error("erreur écriture contenu \n");
		  			}
		  		}
		  	}
    
    
    
  		}	
  	}
  remove("../maps/saved.map");
  rename("new.map", "../maps/saved.map");
  }
  close(new);
  close(fd);
  return 0;
}
