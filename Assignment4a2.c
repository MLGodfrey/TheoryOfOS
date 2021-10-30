#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t* cond = NULL;

int threads;
int loops;
volatile int count = 0;
void* sync_thread(void* num){ //syncs threads
  int thread_number = *(int*)num;
  while(1){
    pthread_mutex_lock(&mutex); //lock
    if(thread_number != count){ //if thread number != count, wait
      pthread_cond_wait(&cond[thread_number], &mutex);
    }
    printf("%d", thread_number + 1); //print thread number
    count = (count+1)%(threads);
    pthread_cond_signal(&cond[count]);//signal next thread
    pthread_mutex_unlock(&mutex);
    }
    return NULL;
  }
int main(){
  pthread_t* thread_id;
  volatile int i;
  int* thread_arr;
  printf("\nEnter number of threads: ");
  scanf("%d", &threads);
  cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t) * threads); //alloc memory
  thread_id = (pthread_t*)malloc(sizeof(pthread_t) * threads);
  thread_arr = (int*)malloc(sizeof(int) * threads);

  //printf("\nEnter number of loops: ");
  //scanf("%d", &loops);
  //for(int j = 0; j < loops; j++){
  for(int i = 0; i < threads; i++){
    pthread_cond_init(&cond[i], NULL);
  }

  for(i = 0; i < threads; i++){
    thread_arr[i] = i;
    pthread_create(&thread_id[i], NULL, sync_thread, (void*)&thread_arr[i]);
  }
  //wait for thread
  for(i = 0; i < threads; i++){
    pthread_join(thread_id[i],NULL);
  }

  return 0;
//}

}
