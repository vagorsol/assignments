#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100000

pthread_mutex_t mutex;

struct thread_args{
  int min;
  int max;
  int* v;
  int* u;
  int* dotproduct; 
};

void *dotbot(void* args){
  struct thread_args *vals = args;

  for(int i = vals->min; i < vals->max; i++){
    pthread_mutex_lock(&mutex);
    vals->dotproduct += vals->u[i] * vals->v[i];
    pthread_mutex_unlock(&mutex);
  }
  return (void *)0;
}

int main(int argc, char *argv[]) {
  srand(time(0));

  int v[SIZE];
  int u[SIZE];
  int dotproduct = 0;
   
  for (int i = 0; i < SIZE; i++) {
    v[i] = rand() % 1000 - 500;
    u[i] = rand() % 1000 - 500;
    dotproduct += u[i] * v[i];
  }
  printf("Ground truth dot product: %d\n", dotproduct);

  pthread_t threads[4];
  struct thread_args args[4];

  dotproduct = 0;
  pthread_mutex_init(&mutex, NULL);
  for(int i = 0; i < 4; i++){
    args[i].min = (SIZE / 4) * i;
    args[i].max = (SIZE / 4) + (SIZE / 4) * i;
    args[i].u = u;
    args[i].v = v;
    args[i].dotproduct = &dotproduct; 
    pthread_create(&threads[i], NULL, dotbot, &args[i]);
  }

  for(int i = 0; i < 4; i++){
    pthread_join(threads[i], NULL);
  }
  printf("Test with 4 threads\n");
  printf("Answer with threads: %d\n", dotproduct);
  pthread_mutex_destroy(&mutex);
  return 0;
}
