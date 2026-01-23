#ifndef WORKER_H
#define WORKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>

#define ROLL_LAST_DIGIT 9
#define ITERATIONS (ROLL_LAST_DIGIT * 1000)

static void cpu_worker() {
    volatile double result = 0.0;
    
    // 2. We use a very large limit to ensure it runs for 5-8 seconds
    long limit = (long)ITERATIONS * 10000L; 
    
    for (long i = 0; i < limit; i++) {
        // 3. Stick to sqrt and tan as requested
        result += sqrt(i) + tan(i);
    }
    
    // 4. Remove any printf from here to keep your terminal clean
    (void)result; 
}

static void mem_worker() {
    size_t s= 1024 *1024*50;
    
    for(int i=0;i<ITERATIONS/5;i++) {
        int *array=(int*)calloc(s,sizeof(int));
        if(array){
            memset(array,1,s*sizeof(int));
            usleep(2000); 
            free(array);
        }
    }
}

static void io_worker() {
    char fname[32];
    sprintf(fname,"inOut_%d.dat",getpid());
    int fd=open(fname, O_WRONLY |O_CREAT|O_TRUNC,0644);

    if (fd!=-1){
        const char *data="IO_DATA_BLOCK"; 
        size_t len = strlen(data);

        for (int i=0;i<ITERATIONS;i++) {
            ssize_t ret =write(fd,data,len);
            (void)ret; 
        }
        fsync(fd);
        close(fd);
    }
    else
        printf("file can't open.");
}

static void run_worker(const char *type){
    if(strcmp(type,"cpu")==0) 
        cpu_worker();
    else if (strcmp(type, "mem") == 0) 
        mem_worker();
    else if (strcmp(type,"io") == 0) 
        io_worker();
    else 
        printf("Intensive should be cpu|mem|io.");

}

#endif