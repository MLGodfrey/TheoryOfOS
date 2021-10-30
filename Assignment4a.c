#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int counter = 0;
int numThreads = 0;
int lock = 0;

void acquire(){
  while (lock) ;
  lock = 1;
}

void release(){
  lock = 0;
}

void *worker_thread(void *arg){
  acquire();
  for(int i = 0; i < numThreads; i++){
    counter++;
  }
  printf("%i\n", counter);
  release();
  return 0;
}

int main(){
  int num_of_loops = 0;
  printf("Enter number of threads: ");
  scanf("%d", &numThreads);
  fflush(stdin);
  printf("Enter number of times the counter shall loop: ");
  scanf("%d", &num_of_loops);

  pthread_t * thread = malloc(sizeof(pthread_t) * numThreads);

  for(int i = 0; i < numThreads; i++) {
    pthread_create(&thread[i], NULL, worker_thread, NULL);
  }
  for (int i = 0; i < numThreads; i++){
    pthread_join(thread[i], NULL);
  }
}
