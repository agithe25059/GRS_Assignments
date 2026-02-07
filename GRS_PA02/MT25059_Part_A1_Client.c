#include "MT25059_common.h"
#include <pthread.h>

typedef struct {
    int msg_size;
    int duration;
    long long bytes_received;
} ThreadArgs;

void *client_thread(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *buffer = malloc(args->msg_size);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error in creating the socket.");
        return NULL;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Not a valid Address");
        return NULL;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("failed in establishing the connecttion.");
        return NULL;
    }

    send(sock, &args->msg_size, sizeof(int), 0);

    time_t start = time(NULL);
    while (difftime(time(NULL), start) < args->duration) {
        ssize_t valread = recv(sock, buffer, args->msg_size, 0);
        if (valread <= 0) break;
        args->bytes_received += valread;
    }

    close(sock);
    free(buffer);
    return NULL;
}

int main(int argc,char *argv[]) {
    if (argc != 4) {
        printf("need to be in this format: %s <msg_size> <threads> <duration>\n", argv[0]);
        return -1;
    }

    int msg_s = atoi(argv[1]),num_threads = atoi(argv[2]),duration = atoi(argv[3]);

    pthread_t threads[num_threads];
    ThreadArgs args[num_threads];

    for(int i=0; i<num_threads;i++) {
        args[i].msg_size=msg_s;
        args[i].duration=duration;
        args[i].bytes_received = 0;
        pthread_create(&threads[i], NULL, client_thread, &args[i]);
    }

    long long tot_bytes=0;
    for(int i=0;i< num_threads;i++) {
        pthread_join(threads[i], NULL);
        tot_bytes =tot_bytes+args[i].bytes_received;
    }

    double throughput_gbps=(tot_bytes* 8.0)/(duration*1e9);
    printf("Total Throughput: %.4f Gbps\n", throughput_gbps);

    return 0;
}