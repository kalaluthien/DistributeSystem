#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <emmintrin.h>

#define FNUM 5
#define TNUM 8
#define COUNT 1000000000
#define LOCK "lock; "
#define ENDL "\n\t"

void *work_bts(void *vargp);
void *work_cas(void *vargp);
void *work_faa(void *vargp);
void *work_mmf(void *vargp);
void *work_stf(void *vargp);

int lock;
int mem;
int tnum = TNUM;
int count = COUNT;
pthread_t tid[TNUM];
void *(*tfunc[FNUM])(void *) = {
  work_bts,
  work_cas,
  work_faa,
  work_mmf,
  work_stf
};
const char *fname[FNUM] = {
  "bts",
  "cas",
  "faa",
  "mmf",
  "stf"
};

int main() {
  int i, j;
  time_t start_time, end_time;
  double elapsed_time;

  /* Sequential code */
  start_time = time(NULL);
  while (mem < COUNT) {
    mem += 1;
  }
  end_time = time(NULL);

  elapsed_time = difftime(end_time, start_time);
  fprintf(stdout, "[%s] elapsed: %.0lf sec\nshared memory: %d / %d\n",
          "seq", elapsed_time, mem, COUNT);
  mem = 0;

  /* Parallel code */
  for (j = 0; j < FNUM; j++) {
    for (i = 0; i < TNUM; i++) {
      pthread_create(&tid[i], NULL, tfunc[j], (void *) ((long long int) i));
    }

    start_time = time(NULL);
    for (i = 0; i < TNUM; i++) {
      void * ret;
      pthread_join(tid[i], &ret);
    }
    end_time = time(NULL);

    elapsed_time = difftime(end_time, start_time);
    fprintf(stdout, "[%s] elapsed: %.0lf sec\nshared memory: %d / %d\n",
            fname[j], elapsed_time, mem, COUNT);
    mem = 0;
  }

  return 0;
}

void *work_bts(void *vargp) {
  __asm__ __volatile__(
    "L_loop%=:" ENDL
    LOCK "bts $0, %0" ENDL
    "jc L_loop%=" ENDL
    "movl %1, %%eax" ENDL
    "cmpl %2, %%eax" ENDL
    "jge L_done%=" ENDL
    "incl %1" ENDL
    "movl $0, %0" ENDL
    "jmp L_loop%=" ENDL
    "L_done%=:" ENDL
    "movl $0, %0" ENDL
    : "+m"(lock), "+m"(mem), "+m"(count)
  );
 
  return NULL;
}

void *work_cas(void *vargp) {
  __asm__ __volatile__(
    "L_loop%=:" ENDL
    "movl %0, %%eax" ENDL
    "cmpl %1, %%eax" ENDL
    "jge L_done%=" ENDL
    "movl %%eax, %%edx" ENDL
    "incl %%edx" ENDL
    LOCK "cmpxchgl %%edx, %0" ENDL
    "jz L_loop%=" ENDL
    "L_done%=:" ENDL
    : "+m"(mem), "+m"(count)
  );

  return NULL;
}

void *work_faa(void *vargp) {
  int i = *((int *) &vargp);
  int load = (COUNT / TNUM) + (i < (COUNT % TNUM) ? 1 : 0);

  __asm__ __volatile__(
    "movl %1, %%edx" ENDL
    "L_loop%=:" ENDL
    "cmpl $0, %%edx" ENDL
    "jle L_done%=" ENDL
    "decl %%edx" ENDL
    LOCK "addl $1, %0" ENDL
    "jmp L_loop%=" ENDL
    "L_done%=:" ENDL
    : "+m"(mem), "+r"(load)
  );

  return NULL;
}

void *work_mmf(void *vargp) {
  int i = *((int *) &vargp);

  __asm__ __volatile__(
    "L_loop%=:" ENDL
    "mfence" ENDL
    "movl %0, %%ecx" ENDL
    "cmpl %1, %%ecx" ENDL
    "jge L_done%=" ENDL
    "movl %%ecx, %%eax" ENDL
    "cltd" ENDL
    "idivl %2" ENDL
    "cmpl %3, %%edx" ENDL
    "jne L_loop%=" ENDL
    "incl %0" ENDL
    "jmp L_loop%=" ENDL
    "L_done%=:" ENDL
    : "+m"(mem), "+m"(count), "+m"(tnum), "+r"(i)
  );

  return NULL;
}

void *work_stf(void *vargp) {
  int i = *((int *) &vargp);

  __asm__ __volatile__(
    "L_loop%=:" ENDL
    "sfence" ENDL
    "movl %0, %%ecx" ENDL
    "cmpl %1, %%ecx" ENDL
    "jge L_done%=" ENDL
    "movl %%ecx, %%eax" ENDL
    "cltd" ENDL
    "idivl %2" ENDL
    "cmpl %3, %%edx" ENDL
    "jne L_loop%=" ENDL
    "incl %0" ENDL
    "jmp L_loop%=" ENDL
    "L_done%=:" ENDL
    : "+m"(mem), "+m"(count), "+m"(tnum), "+r"(i)
  );

  return NULL;
}
