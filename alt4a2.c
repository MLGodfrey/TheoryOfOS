#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common.h"
#include "common_threads.h"

int volatile counter = 0;
int N;
int T;
pthread_mutex_t mut;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int done =0;

void *mythread(void *arg) {
    //lock thread
    printf("thread %c ",arg);
    //sleep(1);
    pthread_mutex_lock(&mut);
 
    int localCounter =0;

    for(int i =0; i < N; i++){
        //counter = counter +1;
        localCounter = localCounter +1;
        //printf("%d ",i);
    }
    printf("\n");
    //printf("\nthread %c --- counter value:%d\n", arg,localCounter);
    //unlock thread
    done =1;

    //signals thread has completed and no longer needs others to wait
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mut);
    return NULL;
}

int main(int argc, char *argv[]) {                    
    if (argc != 3) {
    fprintf(stderr, "usage: ./thread1 N T - (N)umber of loops (T)hreads to create\n");
    exit(1);
    }

    N = atoi(argv[1]);
    T = atoi(argv[2]);


    printf("number of loops: %d\nNumber of threads: %d\n",N,T);

    pthread_t tid[T];
    for(int j = 0; j<N; j++){
        for(int i = 0; i < T; i++){
            int val = i;

            Pthread_create(&tid[i],NULL,mythread, (val+65));
            
            Mutex_lock(&mut);
            //checks if the global var 'done' is set to a certain value that changes in the thread
            while (done == 0){ 
                pthread_cond_wait(&cond, &mut); // releases lock when going to sleep
            }
            Mutex_unlock(&mut);
            done = 0;
        }
    
    //pthread_cond_signal(&cond);
    }
    for(int j =0; j<T;j++){
        Pthread_join(tid[j],NULL);
    }
    printf("main: end\n");
    //printf("counter final = %d",counter);
    return 0;
}
