
#include "MT25059_common.h"
#include <pthread.h>
#include <sys/uio.h> 

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

    struct iovec iov[8];
    struct msghdr message_header;

    for(int i=0; i<8; i++) {
        iov[i].iov_base = msg.fields[i];
        iov[i].iov_len = msg.sizes[i];
    }

    memset(&message_header, 0, sizeof(message_header));
    message_header.msg_iov = iov;
    message_header.msg_iovlen = 8;

    while(1){
        ssize_t sent = sendmsg(client_sock, &message_header, MSG_NOSIGNAL); 
        if (sent <= 0) break;
    }

    free_complex_message(&msg);
    close(client_sock);
    return NULL;
}

int main(){
    signal(SIGPIPE, SIG_IGN);

    int server_fd, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd=socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
        perror("socket failed"); exit(EXIT_FAILURE); 
    }
    int opt=1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET; address.sin_addr.s_addr = INADDR_ANY; address.sin_port = htons(PORT);

    if (bind(server_fd,(struct sockaddr *)&address, sizeof(address)) < 0){
         perror("bind failed"); exit(EXIT_FAILURE); 
    }
    if (listen(server_fd, 10)<0){ 
        perror("listen"); exit(EXIT_FAILURE); 
    }

    printf("A2 (One-Copy) Server listening on port %d...\n", PORT);

    while(1){
        new_sock=malloc(sizeof(int));
        if ((*new_sock = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
            perror("accept"); free(new_sock); 
            continue;
        }
        pthread_t t_id;
        pthread_create(&t_id, NULL,handle_client,(void*)new_sock);
        pthread_detach(t_id);
    }
    return 0;
}