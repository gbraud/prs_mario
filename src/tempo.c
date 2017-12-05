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
  encours=encours->suivant;
  if(encours!=NULL){
    timer_set(encours->delai_restant,encours->param);
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
  parameter=param;
  unsigned long current_time=get_time();

  Liste *delais = malloc(sizeof(Liste));
  if(delais==NULL){
    exit_with_error("erreur creation delais");
  }
  delais->depart=current_time;
  delais->delai_restant = delay;
  delais->param=parameter;
  delais->suivant=NULL;

  if(encours==NULL){
    encours=delais;
    struct itimerval timer;
    timer.it_interval.tv_sec=0;
    timer.it_interval.tv_usec=0;
    timer.it_value.tv_sec=delais->delai_restant/1000;
    timer.it_value.tv_usec=((delais->delai_restant)%1000)*1000;
    setitimer(ITIMER_REAL,&timer,NULL);
  }

  else{
    Liste *tmp=encours;
    unsigned long d;
    d=(tmp->depart+tmp->delai_restant)-current_time;
    if(d<delais->delai_restant){
      delais->delai_restant-=d;
      while(tmp->suivant!=NULL && delais->delai_restant>tmp->suivant->delai_restant){
        tmp=tmp->suivant;
        delais->delai_restant-=tmp->delai_restant;
      }
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
    else{
      tmp->delai_restant-=delais->delai_restant;
      delais->suivant=tmp;
      encours=delais;
      struct itimerval timer;
      timer.it_interval.tv_sec=0;
      timer.it_interval.tv_usec=0;
      timer.it_value.tv_sec=encours->delai_restant/1000;
      timer.it_value.tv_usec=((encours->delai_restant)%1000)*1000;
      setitimer(ITIMER_REAL,&timer,NULL);
    }

  }

  return (timer_id_t) NULL;
}

int timer_cancel (timer_id_t timer_id)
{
  // TODO

  return 0; // failure
}

#endif
