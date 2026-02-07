#include "MT25059_common.h"
#include <pthread.h>

void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    free(arg);
    
    int msg_size = 0;
    if (read_n(client_sock, &msg_size, sizeof(int)) <= 0) {
        close(client_sock);
        return NULL;
    }

    ComplexMessage msg;
    allocate_complex_message(&msg, msg_size);

    char *flat_buffer = (char *)malloc(msg_size);
    int offset = 0;
    for(int i=0; i<8; i++) {
        memcpy(flat_buffer + offset, msg.fields[i], msg.sizes[i]);
        offset += msg.sizes[i];
    }

    while (1) {
        ssize_t sent = send(client_sock, flat_buffer, msg_size, MSG_NOSIGNAL); 
        if (sent <= 0) break;
    }

    free(flat_buffer);
    free_complex_message(&msg);
    close(client_sock);
    return NULL;
}

int main() {
    signal(SIGPIPE, SIG_IGN);

    int server_fd,*new_sock;
    struct sockaddr_in address;
    int addrlen=sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error in creating the socket.");
        exit(EXIT_FAILURE);
    }

    int opt=1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0) {
        perror("failed in bind");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 10)<0) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    printf("A1 Server listening on port %d...\n",PORT);

    while(1){
        new_sock=malloc(sizeof(int));
        if ((*new_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            free(new_sock);
            continue;
        }
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)new_sock) < 0) {
            perror("pthread_create error");
            free(new_sock);
        }
        pthread_detach(thread_id);
    }
    return 0;
}