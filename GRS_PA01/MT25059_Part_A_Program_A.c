#include "MT25059_Part_B_worker.h"
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Enter command-line argumnets in the format of : %s <cpu|mem|io> no.of processes.\n", argv[0]);
        return 1;
    }
    int n=atoi(argv[1]);
    char *type=argv[2];

    for(int i=n-1;i>=0;--i) {
        if (fork()==0) {
            run_worker(type);
            exit(0);
        }
    }
    for(int i =n-1;i>=0;i--) 
        wait(NULL);
    return 0;
}