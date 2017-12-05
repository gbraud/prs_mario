#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../include/error.h"
#include "../include/map.h"

void getWidth(int fd){
  int width;
  if(read(fd,&width,sizeof(int))==-1)
    exit_with_error("erreur lecture width\n");
  printf("width : %d \n",width);
}

void getHeight(int fd){
  int height;
  lseek(fd,sizeof(int),SEEK_SET);
  if(read(fd,&height,sizeof(int))==-1)
    exit_with_error("erreur lecture height\n");
  printf("height : %d \n",height);
}

void getObjects(int fd){
  int nb;
  int taille;
  char c;
  lseek(fd,2*sizeof(int),SEEK_SET);
  if(read(fd,&nb,sizeof(int))==-1)
    exit_with_error("erreur lecture nombre d'objets\n");
  printf("nombre d'objets : %d \n", nb);
  for(int i=0;i<nb;i++){
    if(read(fd,&taille,sizeof(int))==-1)
      exit_with_error("erreur lecture taille\n");
    printf("taille nom : %d\n",taille);
    for(int j=0;j<taille;j++){
      if(read(fd,&c,sizeof(char))==-1)
        exit_with_error("erreur lecture taille\n");
      printf("%c",c );
    }
    printf("\n");
    if(read(fd,&taille,sizeof(int))==-1)
      exit_with_error("erreur lecture taille\n");
    printf("frame : %d\n",taille);
    if(read(fd,&taille,sizeof(int))==-1)
      exit_with_error("erreur lecture prop\n");
    printf("prop : %d\n",taille);
    printf("\n");
  }
}

void setWidth(int fd,int newwidth){
  int new=open("new.map",O_WRONLY | O_CREAT | O_TRUNC,0640);
  if(new==-1)
    exit_with_error("erreur ouverture\n");
  int width, height, taille, nb,buf;
  char c;
  int w=newwidth;
  if(read(fd,&width,sizeof(int))==-1)
    exit_with_error("erreur lecture width\n");
  if(write(new,&w,sizeof(int))!=sizeof(int))
     exit_with_error("erreur écriture nouvelle largeur\n");

  if(read(fd,&height,sizeof(int))==-1)
    exit_with_error("erreur lecture height\n");
  if(write(new,&height,sizeof(int))!=sizeof(int))
     exit_with_error("erreur écriture height\n");

  if(read(fd,&nb,sizeof(int))==-1)
    exit_with_error("erreur lecture nb objets\n");
  if(write(new,&nb,sizeof(int))!=sizeof(int))
    exit_with_error("erreur écriture nb objets\n");

  for(int i=0;i<nb;i++){
    if(read(fd,&taille,sizeof(int))==-1)
    exit_with_error("erreur lecture %d\n",i);
    if(write(new,&taille,sizeof(int))!=sizeof(int))
    exit_with_error("erreur écriture %d\n",i);
    for(int j=0; j<taille; j++){
      if(read(fd, &c, sizeof(char)) == -1)
        exit_with_error("erreur lecture nom %d",i);
      if(write(new,&c,sizeof(char))!=sizeof(char))
        exit_with_error("erreur écriture nom %d\n",i);
    }
    if(read(fd,&buf,sizeof(int))==-1)
    exit_with_error("erreur lecture frame %d\n",i);
    if(write(new,&buf,sizeof(int))!=sizeof(int))
    exit_with_error("erreur écriture frame %d\n",i);
    if(read(fd,&buf,sizeof(int))==-1)
    exit_with_error("erreur lecture prop %d\n",i);
    if(write(new,&buf,sizeof(int))!=sizeof(int))
    exit_with_error("erreur écriture prop %d\n",i);
  }
  if(w<width){
    for(int i=0;i<height;i++){
      for(int j=0;j<w;j++){
        if(read(fd,&buf,sizeof(int))==-1)
          exit_with_error("erreur lecture contenu\n");
        if(write(new,&buf,sizeof(int))!=sizeof(int))
          exit_with_error("erreur écriture contenu \n");
      }
      lseek(fd,(width-w)*sizeof(int),SEEK_CUR);
    }
  }
  else{
    for(int i=0;i<height;i++){
      for(int j=0;j<width;j++){
        if(read(fd,&buf,sizeof(int))==-1)
          exit_with_error("erreur lecture contenu\n");
        if(write(new,&buf,sizeof(int))!=sizeof(int))
          exit_with_error("erreur écriture contenu \n");
      }
      for(int k=width;k<w;k++){
        buf=-1;
        if(write(new,&buf,sizeof(int))!=sizeof(int))
          exit_with_error("erreur écriture contenu supp \n");
      }
    }
  }
  remove("../maps/saved.map");
  rename("new.map", "../maps/saved.map");
  close(new);
  }

void setHeight(int fd, int newheight){
  int new=open("new.map",O_WRONLY | O_CREAT | O_TRUNC,0640);
  if(new==-1)
    exit_with_error("erreur ouverture\n");
  int width, height, taille, nb,buf;
  char c;
  int h=newheight;
  if(read(fd,&width,sizeof(int))==-1)
    exit_with_error("erreur lecture width\n");
  if(write(new,&width,sizeof(int))!=sizeof(int))
     exit_with_error("erreur écriture width\n");

  if(read(fd,&height,sizeof(int))==-1)
    exit_with_error("erreur lecture height\n");
  if(write(new,&h,sizeof(int))!=sizeof(int))
     exit_with_error("erreur écriture nouvelle hauteur\n");

  if(read(fd,&nb,sizeof(int))==-1)
    exit_with_error("erreur lecture nb objets\n");
  if(write(new,&nb,sizeof(int))!=sizeof(int))
    exit_with_error("erreur écriture nb objets\n");

  for(int i=0;i<nb;i++){
  	if(read(fd,&taille,sizeof(int))==-1)
    exit_with_error("erreur lecture %d\n",i);
  	if(write(new,&taille,sizeof(int))!=sizeof(int))
    exit_with_error("erreur écriture %d\n",i);
  	char c;
  	for(int j=0; j<taille; j++){
    	if(read(fd, &c, sizeof(char)) == -1)
      	exit_with_error("erreur lecture nom %d",i);
    	if(write(new,&c,sizeof(char))!=sizeof(char))
    		exit_with_error("erreur écriture nom %d\n",i);
  	}
  	if(read(fd,&buf,sizeof(int))==-1)
    exit_with_error("erreur lecture frame %d\n",i);
  	if(write(new,&buf,sizeof(int))!=sizeof(int))
    exit_with_error("erreur écriture frame %d\n",i);
  	if(read(fd,&buf,sizeof(int))==-1)
    exit_with_error("erreur lecture prop %d\n",i);
  	if(write(new,&buf,sizeof(int))!=sizeof(int))
    exit_with_error("erreur écriture prop %d\n",i);
  }
  if(h<height){
  	lseek(fd,(height-h)*width*sizeof(int),SEEK_CUR);
  	for(int i=0;i<h;i++){
  		for(int j=0;j<width;j++){
    		if(read(fd,&buf,sizeof(int))==-1)
      		exit_with_error("erreur lecture contenu\n");
    		if(write(new,&buf,sizeof(int))!=sizeof(int))
    			exit_with_error("erreur écriture contenu \n");
  		}
		}
  }
  else{
  	for(int i=0;i<h-height;i++){
  		for(int j=0;j<width;j++){
  			buf=-1;
  			if(write(new,&buf,sizeof(int))!=sizeof(int))
    			exit_with_error("erreur écriture contenu supp \n");
  		}
  	}
  	for(int k=0;k<height;k++){
  		for(int j=0;j<width;j++){
  			if(read(fd,&buf,sizeof(int))==-1)
      		exit_with_error("erreur lecture contenu\n");
    		if(write(new,&buf,sizeof(int))!=sizeof(int))
    			exit_with_error("erreur écriture contenu \n");
  		}
  	}
  }
  remove("../maps/saved.map");
  rename("new.map", "../maps/saved.map");
  close(new);
}

void setObjects(int fd,int nbarg,char** argv){
  int new=open("new.map",O_WRONLY | O_CREAT | O_TRUNC,0640);
  if(new==-1)
    exit_with_error("erreur ouverture\n");
  int width, height, taille, nb,buf;
  char c;
  int new_nb_objects = (nbarg - 3)/6;
  if(read(fd,&width,sizeof(int))==-1)
    exit_with_error("erreur lecture width\n");
  if(write(new,&width,sizeof(int))!=sizeof(int))
     exit_with_error("erreur écriture width\n");

  if(read(fd,&height,sizeof(int))==-1)
    exit_with_error("erreur lecture height\n");
  if(write(new,&height,sizeof(int))!=sizeof(int))
     exit_with_error("erreur écriture nouvelle hauteur\n");

  lseek(fd,sizeof(int),SEEK_CUR);
  if(write(new,&new_nb_objects,sizeof(int))!=sizeof(int))
    exit_with_error("erreur écriture nb objets\n");
  for(int i = 3 ; i < nbarg ; i += 6){
    char * nom = argv[i];
    taille = strlen(nom);

    if(write(new,&taille,sizeof(int))!= sizeof(int))
      exit_with_error("erreur écriture taille nom");
    if(write(new,nom,sizeof(char)*taille)!= sizeof(char)*taille)
      exit_with_error("erreur écriture nom");

    int frame= atoi(argv[i+1]);
    if(write(new,&frame,sizeof(int))!= sizeof(int))
      exit_with_error("erreur écriture frame");

    int solidity=0;
    int destructible=0;
    int collectible=0;
    int generator=0;


    if(strcmp(argv[i+2],"air")==0){
      solidity = MAP_OBJECT_AIR;
    }
    else if(strcmp(argv[i+2],"semi-solid")==0){
      solidity = MAP_OBJECT_SEMI_SOLID;
    }
    else if(strcmp(argv[i+2],"solid")==0){
      solidity = MAP_OBJECT_SOLID;
    }
    else if(strcmp(argv[i+2],"liquid")==0){
      solidity = MAP_OBJECT_LIQUID;
    }
    else{
      exit_with_error("erreur lecture solidité de l'objet");
    }



    if(strcmp(argv[i+3],"destructible")==0){
      destructible = MAP_OBJECT_DESTRUCTIBLE;
    }
    else if(strcmp(argv[i+3],"not-destructible")==0){
      destructible = 0;
    }
    else{
      exit_with_error("erreur lecture destruct de l'objet");
    }



    if(strcmp(argv[i+4],"collectible")==0){
      collectible = 1;
    }
    else if(strcmp(argv[i+4],"not-collectible")==0){
    }
    else{
      exit_with_error("erreur lecture collect de l'objet");
    }



    if(strcmp(argv[i+5],"generator")==0){
      generator = 1;
    }
    else if(strcmp(argv[i+5],"not-generator")==0){
      generator = 0;
    }
    else{
      exit_with_error("erreur lecture gener de l'objet");
    }


    int prop=solidity
                |destructible << 6
                |collectible<<7
                |generator<<8;

    printf("1 %d\n",solidity );
    printf("2 %d\n",destructible );
    printf("3 %d\n",collectible);
    printf("4 %d\n",generator);
    if(write(new,&prop,sizeof(int))!= sizeof(int))
      exit_with_error("erreur écriture propriétés");
  }

  lseek(fd,-(sizeof(int)*height*width),SEEK_END);
  for(int i = 0 ; i < height*width ; i++){
    if(read(fd,&buf,sizeof(int))==-1)
      exit_with_error("erreur lecture contenu de la map\n");
    if(write(new,&buf,sizeof(int))!=sizeof(int))
       exit_with_error("erreur écriture contenu de la map\n");
  }
  remove("../maps/saved.map");
  rename("new.map", "../maps/saved.map");
  close(new);
}

void pruneObjects(int fd){
  int new=open("new.map",O_WRONLY | O_CREAT | O_TRUNC,0640);
  if(new==-1)
    exit_with_error("erreur ouverture\n");
  int width, height, taille, nb,buf;
  char c;
  if(read(fd,&width,sizeof(int))==-1)
    exit_with_error("erreur lecture fd\n");
  if(write(new,&width,sizeof(int))!=sizeof(int))
    exit_with_error("erreur écriture new\n");

  if(read(fd,&height,sizeof(int))==-1)
    exit_with_error("erreur lecture fd\n");
  if(write(new,&height,sizeof(int))!=sizeof(int))
    exit_with_error("erreur écriture new\n");

  if(read(fd,&nb,sizeof(int))==-1)
    exit_with_error("erreur lecture fd\n");
  for(int i=0;i<nb;i++){
    if(read(fd,&taille,sizeof(int))==-1)
      exit_with_error("erreur lecture fd\n");
    lseek(fd,taille*sizeof(char),SEEK_CUR);
    lseek(fd,2*sizeof(int),SEEK_CUR);
  }
  int occ[nb];
  for(int i=0;i<nb;i++){
    occ[i]=0;
  }
  for(int i=0;i<width*height;i++){
    if(read(fd,&buf,sizeof(int))==-1)
      exit_with_error("erreur lecture fd\n");
    occ[buf]+=1; //occ[buf] >0 si l'élément buf apparait dans la matrice
  }
  int compte=0;
  for(int i=0;i<nb;i++){
    if(occ[i]==0)
      compte+=1;
  }
  lseek(fd,3*sizeof(int),SEEK_SET);
  int newnb=nb-compte;
  if(write(new,&newnb,sizeof(int))!=sizeof(int))
    exit_with_error("erreur écriture new\n");
  for(int i=0;i<nb;i++){
    if(occ[i]==0){
      if(read(fd,&taille,sizeof(int))==-1)
        exit_with_error("erreur lecture fd\n");
      lseek(fd,taille*sizeof(char),SEEK_CUR);
      lseek(fd,2*sizeof(int),SEEK_CUR);
    }
    else{
      if(read(fd,&taille,sizeof(int))==-1)
        exit_with_error("erreur lecture fd\n");
      if(write(new,&taille,sizeof(int))!=sizeof(int))
        exit_with_error("erreur écriture new\n");
      for(int i=0;i<taille;i++){
        if(read(fd,&c,sizeof(char))==-1)
          exit_with_error("erreur lecture fd\n");
        if(write(new,&c,sizeof(char))!=sizeof(char))
          exit_with_error("erreur écriture new\n");
      }
      if(read(fd,&buf,sizeof(int))==-1)
        exit_with_error("erreur lecture fd\n");
      if(write(new,&buf,sizeof(int))!=sizeof(int))
        exit_with_error("erreur écriture new\n");

      if(read(fd,&buf,sizeof(int))==-1)
        exit_with_error("erreur lecture fd\n");
      if(write(new,&buf,sizeof(int))!=sizeof(int))
        exit_with_error("erreur écriture new\n");
    }
  }

  for(int i=0;i<width*height;i++){ //écriture contenu
    if(read(fd,&buf,sizeof(int))==-1)
      exit_with_error("erreur lecture fd\n");
    if(write(new,&buf,sizeof(int))!=sizeof(int))
      exit_with_error("erreur écriture new\n");
  }
  remove("../maps/saved.map");
  rename("new.map", "../maps/saved.map");
  close(new);
}

int main(int argc, char * argv[]){


  int fd=open(argv[1],O_RDWR,0640);
  if(fd==-1){
    exit_with_error("erreur ouverture fd\n");
  }

  if(strcmp(argv[2],"--getwidth")==0)
    getWidth(fd);

  else if(strcmp(argv[2],"--getheight")==0)
    getHeight(fd);

  else if(strcmp(argv[2],"--getobjects")==0)
    getObjects(fd);

  else if(strcmp(argv[2],"--getinfo")==0){
    getWidth(fd);
    getHeight(fd);
    getObjects(fd);
  }

  else if(strcmp(argv[2],"--setwidth")==0)
    setWidth(fd,atoi(argv[3]));

  else if(strcmp(argv[2],"--setheight")==0)
    setHeight(fd,atoi(argv[3]));

  else if(strcmp(argv[2],"--setobjects")==0)
    setObjects(fd,argc,argv);

  else if(strcmp(argv[2],"--pruneobjects")==0)
    pruneObjects(fd);

  close(fd);
  return 0;
}
