#include "SDL.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

#include "timer.h"

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

void c(int s){
  printf("Id thread courant : %lu\n",pthread_self());
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
  //printf("%d\n",sig );
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
  // TODO

  return (timer_id_t) NULL;
}

int timer_cancel (timer_id_t timer_id)
{
  // TODO

  return 0; // failure
}

#endif
