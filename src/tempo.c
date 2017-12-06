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

/*Component of the chained waiting list of timer.
  - Time when timer_set is called
  - 
  - The function which be called when the timer will be finished.
  - The next timer which should be launched after this one.
*/
typedef struct info_timer info_timer;
struct info_timer
{
  Uint32 time_called;
  Uint32 initial_delay;
  Uint32 effective_delay;
  void *function;
  info_timer *next_timer;
};

//Function called when a SIGALRM is received.


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



info_timer *current_timer=NULL;

void timer_launcher(){
  struct itimerval timer;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 0;
  timer.it_value.tv_sec = current_timer->effective_delay/1000;
  timer.it_value.tv_usec = (current_timer->effective_delay%1000)*1000;
  setitimer(ITIMER_REAL,&timer,NULL);
}


void add_timer(info_timer *placed, info_timer *to_place, Uint32 current_time){

  Uint32 placed_time_left = (placed->time_called + placed->effective_delay) - current_time;
  Uint32 placed_end = placed->time_called + placed->effective_delay;
  
  //Uint32 to_place_time_left = (to_place->time_called + to_place->effective_delay) - current_time;
  Uint32 to_place_end = to_place->time_called + to_place->effective_delay;
  

  if( placed_end < to_place_end){
    to_place->time_called = placed_end;
    to_place->effective_delay -= placed_time_left;
    if(placed->next_timer == NULL){
      placed->next_timer = to_place;
    }
    else{
      add_timer(placed->next_timer,to_place,current_time);
    }
  }
  else{

    if(placed == current_timer){
      placed->time_called = to_place_end;
      placed->effective_delay -= (current_time - placed->time_called) + to_place->effective_delay;
      to_place->next_timer = placed;
      current_timer = to_place;
      timer_launcher();
    }
    else{
      info_timer *previous = current_timer;
      while(previous->next_timer != placed){
        previous = previous->next_timer;
      }
      placed->effective_delay -= to_place->effective_delay;
      placed->time_called += to_place->effective_delay;
      to_place->next_timer = placed;
      previous->next_timer = to_place;
    }
  }
}

  


void signal_treatment(){
  if(current_timer!=NULL){
    
    info_timer *backup = current_timer;
    void* function = current_timer->function;
    current_timer=current_timer->next_timer;
    if(current_timer != NULL){
      timer_launcher();
    }


    sdl_push_event(function);
    
    printf ("sdl_push_event(%p) appelée au temps : %d ms.\n", function,time);

    free(backup);
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
  s.sa_handler = signal_treatment;
  sigemptyset(&s.sa_mask);
  s.sa_flags=0;
  sigaction(SIGALRM,&s,NULL);

  pthread_create(&tid,NULL,routine,&m);
  return 1; // Implementation not ready
}


timer_id_t timer_set (Uint32 delay, void *param)
{
  printf("APPEL DE TIMER_SET et current_timer = %d\n",current_timer);
  //Time in ms.
  Uint32 current_time=get_time()/1000;

  info_timer *new_timer = malloc(sizeof(info_timer));
  if(new_timer == NULL){
    exit_with_error("Error access memory (creation new info_timer)");
  }
  else{
    new_timer->time_called = current_time;
    new_timer->initial_delay = delay;
    new_timer->effective_delay = delay;
    new_timer->function = param;
    new_timer->next_timer = NULL;
  }

//If no timer is running.
//OK

  if(current_timer == NULL){
    current_timer = new_timer;

    timer_launcher();
  }
//If a timer is already running.
  else{
    add_timer(current_timer,new_timer,current_time);
  }
 
  return (timer_id_t) NULL;
}

int timer_cancel (timer_id_t timer_id)
{
  // TODO

  return 0; // failure
}

#endif
