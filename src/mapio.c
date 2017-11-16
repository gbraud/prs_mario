#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "map.h"
#include "error.h"

#ifdef PADAWAN

void map_new (unsigned width, unsigned height)
{
  map_allocate (width, height);

  for (int x = 0; x < width; x++)
    map_set (x, height - 1, 0); // Ground

  for (int y = 0; y < height - 1; y++) {
    map_set (0, y, 1); // Wall
    map_set (width - 1, y, 1); // Wall
  }

  map_object_begin (8);

  // Texture pour le sol
  map_object_add ("images/ground.png", 1, MAP_OBJECT_SOLID);
  // Mur
  map_object_add ("images/wall.png", 1, MAP_OBJECT_SOLID);
  // Gazon
  map_object_add ("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
  // Marbre
  map_object_add ("images/marble.png", 1, MAP_OBJECT_SOLID | MAP_OBJECT_DESTRUCTIBLE);
  // Herbe
  map_object_add ("images/herb.png", 1, MAP_OBJECT_AIR);
  // Petit plancher flottant
  map_object_add ("images/floor.png", 1, MAP_OBJECT_SEMI_SOLID);

  map_object_add ("images/flower.png",1,MAP_OBJECT_AIR);

  map_object_add ("images/coin.png",20,MAP_OBJECT_AIR | MAP_OBJECT_COLLECTIBLE);

  map_object_end ();
}

void map_save (char *filename)
{
  int save=open(filename,O_WRONLY|O_TRUNC|O_CREAT,0666);
  if (save == -1) {
    exit_with_error("erreur ouverture sauvegarde");
  }

  int width=map_width();
  if(write(save,&width,sizeof(int))!=sizeof(int)){
    exit_with_error("erreur écriture width");
  }

  int height=map_height();
  if(write(save,&height,sizeof(int))!=sizeof(int)){
    exit_with_error("erreur écriture height");
  }

  int nb=map_objects();
  if(write(save,&nb,sizeof(int))!=sizeof(int)){
    exit_with_error("erreur écriture nb objets");
  }


  for(int i=0; i<nb; i++){
    int frame=map_get_frames(i);
    if(write(save,&frame,sizeof(int))!= sizeof(int)){
      exit_with_error("erreur écriture frame");
    }

    char * nom = map_get_name(i);
    int taille = strlen(nom);

    if(write(save,&taille,sizeof(int))!= sizeof(int)){
      exit_with_error("erreur écriture taille nom");
    }
    if(write(save,nom,sizeof(char)*taille)!= sizeof(char)*taille){
      exit_with_error("erreur écriture nom");
    }

    int prop=map_get_solidity(i)
                |map_is_destructible(i) << 6
                |map_is_collectible(i)<<7
                |map_is_generator(i)<<8;

    if(write(save,&prop,sizeof(int))!= sizeof(int)){
      exit_with_error("erreur écriture propriétés");
    }

    }

  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      int contenu=map_get(j,i);
      if(write(save,&contenu,sizeof(int))!=sizeof(int)){
        exit_with_error("erreur écriture contenu case");
      }
    }
  }

  printf("saved \n");
  close(save);
}

void map_load (char *filename)
{

  int width,height,nb;

  int fd=open(filename,O_RDONLY);
  if(fd==-1){
    exit_with_error("erreur ouverture fichier");
  }

  if(read(fd,&width,sizeof(int))==-1){
    exit_with_error("erreur lecture width");
  }
  if(read(fd,&height,sizeof(int))==-1){
    exit_with_error("erreur lecture height");
  }
  map_allocate(width,height);

  if(read(fd,&nb,sizeof(int))==-1){
    exit_with_error("erreur lecture nb objets");
  }

  map_object_begin(nb);

  for(int i=0;i<nb;i++){
    int frame;
    if(read(fd,&frame,sizeof(int))==-1){
      exit_with_error("erreur lecture frame");
    }
    int taille;
    if(read(fd,&taille,sizeof(int))==-1){
      exit_with_error("erreur lecture taille nom");
    }

    char c;
    //char nom[taille];

    char* nom = calloc(sizeof(char), (taille+1));

    for(int j=0; j<taille; j++){
      if(read(fd, &c, sizeof(char)) == -1){
        exit_with_error("erreur lecture nom");
      }
      nom[j]=c;
    }

    int prop;
    if(read(fd,&prop,sizeof(int))==-1){
      exit_with_error("erreur lecture propriétés");
    }
    map_object_add(nom,frame,prop);
    free(nom);
  }

  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      int contenu;
      if(read(fd,&contenu,sizeof(int))==-1){
        exit_with_error("erreur lecture contenu de la case");
      }
      map_set(j,i,contenu);
    }
  }
  map_object_end ();
  close(fd);
}

#endif
