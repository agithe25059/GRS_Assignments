#include "MT25059_Part_B_worker.h"
#include <pthread.h>
#include <stdlib.h>

void* thread_func(void* arg){
    run_worker((char*)arg);
    return NULL;
}

int main(int argc,char *argv[]) {
    if(argc != 3){
        printf("Enter command-line argumnets in the format of : %s <cpu|mem|io> no.of threads.\n",argv[0]);
        return 1;
    }
    int n=atoi(argv[1]);
    char *type=argv[2];
    pthread_t threads[n];

    for (int i=0;i< n; i++) 
        pthread_create(&threads[i], NULL,thread_func,type);

    for(int i=0;i<n;i++)
        pthread_join(threads[i],NULL);  //joining all threads

    return 0;
}