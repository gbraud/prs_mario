#include "SDL.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

#include "timer.h"
#include "error.h"

void *parameter;
// Return number of elapsed µsec since... a long time ago
static unsigned long get_time (void)
{
  struct timeval tv;

  gettimeofday (&tv ,NULL);

  // Only count seconds since beginning of 2016 (not jan 1st, 1970)
  tv.tv_sec -= 3600UL * 24 * 365 * 46;

  return tv.tv_sec * 1000000UL + tv.tv_usec;
}

#ifdef PADAWAN

typedef struct Liste Liste;
struct Liste
{
  unsigned long depart;
  Uint32 delai_restant;
  void *param;
  struct Liste *suivant;
};

Liste *encours=NULL;

void c(int s){
  sdl_push_event(parameter);
  printf ("sdl_push_event(%p) appelée au temps %ld\n", parameter, get_time ());
  if(encours!=NULL){
    encours=encours->suivant;
    if(encours != NULL){
      //parameter=encours->param;
      if(encours->delai_restant < 1000){
        sdl_push_event(encours->param);
        encours=encours->suivant;
      }
      printf("encours : %d\n",encours );
      if(encours != NULL)
        timer_set(encours->delai_restant,encours->param);
      printf("TEST4\n");
    }
  }
}


void *routine(void *p){
  sigset_t m= *(sigset_t*)p;
  sigdelset(&m,SIGALRM);
  pthread_sigmask(SIG_SETMASK,&m,NULL);
  while(1){
    sigsuspend(&m);
  }
}

// timer_init returns 1 if timers are fully implemented, 0 otherwise
int timer_init (void)
{
  pthread_t tid;
  sigset_t m;
  sigfillset(&m);
  pthread_sigmask(SIG_SETMASK,&m,NULL);
  struct sigaction s;
  s.sa_handler = c;
  sigemptyset(&s.sa_mask);
  s.sa_flags=0;
  sigaction(SIGALRM,&s,NULL);
  pthread_create(&tid,NULL,routine,&m);
  return 1; // Implementation not ready
}


timer_id_t timer_set (Uint32 delay, void *param)
{
  //parameter=param;
  unsigned long current_time=get_time();

  Liste *delais = malloc(sizeof(Liste));
  if(delais==NULL){
    exit_with_error("erreur creation delais");
  }
  else{
    delais->depart=current_time;
    delais->delai_restant = delay;
    delais->param=param;
    delais->suivant=NULL;
  }

//FONCTIONNE BIEN
  if(encours==NULL){
    encours=delais;
    parameter=encours->param;
    struct itimerval timer;
    timer.it_interval.tv_sec=0;
    timer.it_interval.tv_usec=0;
    timer.it_value.tv_sec=encours->delai_restant/1000;
    timer.it_value.tv_usec=((encours->delai_restant)%1000)*1000;
    setitimer(ITIMER_REAL,&timer,NULL);
  }

//Le cas où un timer est déjà en cours.
  else{
    Liste *tmp=encours;
    unsigned long d;
    /*if(tmp->depart+(tmp->delai_restant*1000)<current_time){
      exit_with_error("MERDE\n");
    }
    else{*/
      d=(tmp->depart+(tmp->delai_restant*1000))-current_time; //usec
      printf("tmp->depart : %lu\n",tmp->depart);
      printf("tmp->delai_restant : %d\n",tmp->delai_restant*1000);
      printf("current_time : %lu\n",current_time);
    //}
    printf("d : %lu\n",d);


//Le cas où le prochain processus doit finir après le processus en cours
    if(d<(delais->delai_restant*1000)){ //usec
      delais->delai_restant-=d/1000;
      while(tmp->suivant!=NULL && delais->delai_restant>tmp->suivant->delai_restant){
        tmp=tmp->suivant;
        delais->delai_restant-=tmp->delai_restant;
      }
      if(tmp->suivant !=NULL){
        tmp->suivant->delai_restant-=delais->delai_restant;
        delais->suivant=tmp->suivant;
        tmp->suivant=delais;
        encours=tmp;

        struct itimerval timer;
        timer.it_interval.tv_sec=0;
        timer.it_interval.tv_usec=0;
        timer.it_value.tv_sec=encours->delai_restant/1000;
        timer.it_value.tv_usec=((encours->delai_restant)%1000)*1000;
        setitimer(ITIMER_REAL,&timer,NULL);
      }
    }
//Le cas où le prochain processus doit finir avant celui en cours
    else{
      d = current_time - tmp->depart;
      tmp->delai_restant -= d/1000;
      tmp->delai_restant -= delais->delai_restant; 

      
      delais->suivant=tmp;
      delais->suivant->delai_restant = tmp->delai_restant;
      encours=delais;


      struct itimerval timer;
      timer.it_interval.tv_sec=0;
      timer.it_interval.tv_usec=0;
      timer.it_value.tv_sec=encours->delai_restant/1000;
      timer.it_value.tv_usec=((encours->delai_restant)%1000)*1000;
      setitimer(ITIMER_REAL,&timer,NULL);
    }

  }
  //free(delais);
  return (timer_id_t) NULL;
}

int timer_cancel (timer_id_t timer_id)
{
  // TODO

  return 0; // failure
}

#endif
