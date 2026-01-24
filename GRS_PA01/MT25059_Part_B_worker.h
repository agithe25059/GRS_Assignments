#ifndef WORKER_H
#define WORKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>

#define ROLL_LAST_DIGIT 9
#define ITERATIONS (ROLL_LAST_DIGIT* 1000)

static void cpu_worker(){
    volatile double result = 0.0;
    long limit = (long)ITERATIONS* 10000L; 
    
    for (long i =0;i<limit;i++)
        result =result+ sqrt(i)+tan(i);
}

static void mem_worker() {
    size_t s= 1024 *1024*50;
    
    for(int i=ITERATIONS/5-1;i>=0;i--) {
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
        size_t len =strlen(data);
        int i=0;
        while(i<ITERATIONS){
            ssize_t ret =write(fd,data,len);
            i++;
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
    else if (strcmp(type, "mem") ==0) 
        mem_worker();
    else if (strcmp(type,"io")== 0) 
        io_worker();
    else 
        printf("Intensive should be cpu|mem|io.");

}

#endif