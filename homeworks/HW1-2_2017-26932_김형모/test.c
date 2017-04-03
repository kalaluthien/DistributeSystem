#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define TNUM 8
#define COUNT 1000000000
#define LOCK_PREFIX "lock; "
#define ENDL "\n\t"

int lock;
int mem;
pthread_t tid[TNUM];

void * work_bts(void * vargp) {
  int count = COUNT;

  while (mem < COUNT) {
    __asm__ __volatile__(
      "L_aquire%=:" ENDL
      LOCK_PREFIX "bts $0, %0" ENDL
      "jc L_aquire%=" ENDL
      "movl %1, %%eax" ENDL
      "cmpl %2, %%eax" ENDL
      "jge L_release%=" ENDL
      "incl %1" ENDL
      "L_release%=:" ENDL
      "movl $0, %0" ENDL
      : "+m"(lock), "+m"(mem), "+m"(count)
    );
  }

  return NULL;
}

void * work_cas(void * vargp) {
  int count = COUNT;

  while (mem < COUNT) {
    __asm__ __volatile__(
      "L_loop%=:" ENDL
      "movl %0, %%eax" ENDL
      "cmpl %1, %%eax" ENDL
      "jge L_done%=" ENDL
      "movl %%eax, %%edx" ENDL
      "incl %%edx" ENDL
      LOCK_PREFIX "cmpxchgl %%edx, %0" ENDL
      "jz L_loop%=" ENDL
      "L_done%=:" ENDL
      : "+m"(mem), "+m"(count)
    );
  }

  return NULL;
}

void * work_add(void * vargp) {
  int i = *((int *) &vargp);
  int load = (COUNT / TNUM) + (i < (COUNT % TNUM) ? 1 : 0);

  while (load--) {
    __asm__ __volatile__(
      LOCK_PREFIX "addl $1, %0" ENDL
      : "+m"(mem)
    );
  }

  return NULL;
}

int main() {
  time_t start_time, end_time;
  double elapsed_time;
  void * (* work)(void *);
  
  work = work_cas;

  start_time = time(NULL);
#ifdef SER
  {
    while (mem < COUNT) {
      mem++;
    }
  }
#else
  {
    long long int i;
    for (i = 0; i < TNUM; i++) {
      pthread_create(&tid[i], NULL, work, (void *) i);
    }

    for (i = 0; i < TNUM; i++) {
      void * ret;
      pthread_join(tid[i], &ret);
    }
  }
#endif
  end_time = time(NULL);

  elapsed_time = difftime(end_time, start_time);
  fprintf(stdout, "elapsed: %.0lf sec\nshared memory: %d / %d\n", elapsed_time, mem, COUNT);

  return 0;
}
