#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define TNUM 16
#define DEFAULT_COUNT 100000000
#define LOCK_PREFIX "lock; "
#define ENDL "\n\t"

#define INIT_SHARED_MEMORY(V)  \
do { \
  lock = (V); \
  stamp = (V); \
  mem = (V);  \
} while(0)

int lock;   // shared lock
int stamp;  // shared timestamp
int mem;    // shared counter
int count;
pthread_t tid[TNUM];

typedef void * (* thread_routine)(void *);

void * work_TAS(void * vargp) {
  __asm__ __volatile__(
    "L_main%=:" ENDL

    "L_aquire%=:" ENDL
    LOCK_PREFIX "bts $0, %0" ENDL
    "jc L_aquire%=" ENDL

    "movl %1, %%eax" ENDL
    "cmpl %2, %%eax" ENDL
    "jge L_release%=" ENDL
    "incl %1" ENDL

    "L_release%=:" ENDL
    "movl $0, %0" ENDL

    "movl %1, %%eax" ENDL
    "cmpl %2, %%eax" ENDL
    "jl L_main%=" ENDL
    : "+m"(lock), "+m"(mem), "+m"(count)
  );

  return NULL;
}

void * work_TTAS(void * vargp) {
  __asm__ __volatile__(
    "L_main%=:" ENDL

    "L_aquire%=:" ENDL
    "L_block%=:" ENDL
    "bt $0, %0" ENDL
    "jc L_block%=" ENDL
    LOCK_PREFIX "bts $0, %0" ENDL
    "jnc L_enter%=" ENDL
    "jmp L_aquire%=" ENDL

    "L_enter%=:" ENDL
    "movl %1, %%eax" ENDL
    "cmpl %2, %%eax" ENDL
    "jge L_release%=" ENDL
    "incl %1" ENDL

    "L_release%=:" ENDL
    "movl $0, %0" ENDL

    "movl %1, %%eax" ENDL
    "cmpl %2, %%eax" ENDL
    "jl L_main%=" ENDL
    : "+m"(lock), "+m"(mem), "+m"(count)
  );

  return NULL;
}

void * work_CAS(void * vargp) {
  __asm__ __volatile__(
    "L_loop%=:" ENDL
    "movl %0, %%eax" ENDL
    "cmpl %1, %%eax" ENDL
    "jge L_done%=" ENDL

    "movl %%eax, %%edx" ENDL
    "incl %%edx" ENDL
    LOCK_PREFIX "cmpxchgl %%edx, %0" ENDL
    "jmp L_loop%=" ENDL
    "L_done%=:" ENDL
    : "+m"(mem), "+m"(count)
  );

  return NULL;
}

void * work_FAA(void * vargp) {
  __asm__ __volatile__(
    "L_main%=:" ENDL
    "movl $1, %%eax" ENDL
    LOCK_PREFIX "xaddl %%eax, %0" ENDL

    "L_aquire%=:" ENDL
    "cmpl %%eax, %1" ENDL
    "jne L_aquire%=" ENDL

    "movl %2, %%eax" ENDL
    "cmpl %3, %%eax" ENDL
    "jge L_release%=" ENDL
    "incl %2" ENDL
    
    "L_release%=:" ENDL
    "addl $1, %1" ENDL

    "movl %2, %%eax" ENDL
    "cmpl %3, %%eax" ENDL
    "jl L_main%=" ENDL
    : "+m"(stamp), "+m"(lock), "+m"(mem), "+m"(count)
  );

  return NULL;
}

thread_routine parse_input(char * str) {
  if (strcmp(str, "TAS") == 0) {
    fprintf(stdout, "Test-And-Set\n");
    return work_TAS;
  }
  if (strcmp(str, "TTAS") == 0) {
    fprintf(stdout, "Test-And-Test-And-Set\n");
    return work_TTAS;
  }
  if (strcmp(str, "CAS") == 0) {
    fprintf(stdout, "Compare-And-Swap\n");
    return work_CAS;
  }
  if (strcmp(str, "FAA") == 0) {
    fprintf(stdout, "Fetch-And-Add\n");
    return work_FAA;
  }

  return NULL;
}

int main(int argc, char * argv[]) {
  time_t start_time, end_time;
  double elapsed_time;

  if (argc == 3) {
    count = atoi(argv[2]);

    if (count < 0) {
      fprintf(stderr, "Error: argument overflow\n");
    }
  }
  else if (argc != 2) {
    fprintf(stderr, "Error: argument number not matched\n");
    exit(-1);
  }
  else {
    count = DEFAULT_COUNT;
  }

  thread_routine work = parse_input(argv[1]);

  if (!work) {
    fprintf(stderr, "Error: invalid argument \"%s\"\n", argv[1]);
    exit(-2);
  }

  long long int i;
  int iteration = 1;

  while (iteration <= TNUM) {
    INIT_SHARED_MEMORY(0);

    start_time = time(NULL);
    {
      for (i = 0; i < iteration; i++) {
        pthread_create(&tid[i], NULL, work, NULL);
      }

      for (i = 0; i < iteration; i++) {
        void * ret;
        pthread_join(tid[i], &ret);
      }
    }
    end_time = time(NULL);

    elapsed_time = difftime(end_time, start_time);
    fprintf(stdout, "  [%d thread]\telapsed: %.0lf sec\n", iteration, elapsed_time);
    fprintf(stdout, "             \tvalidation: %d (%d expected)\n\n", mem, count);

    iteration *= 2;
  }

  fprintf(stdout, "\n");
  return 0;
}
