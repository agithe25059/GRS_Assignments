/*
 * Roll No: MT25059
 * File: MT25059_common.h
 * Description: Common definitions for data structures and socket helpers.
 */

#ifndef MT25059_COMMON_H
#define MT25059_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

#define PORT 8080

typedef struct {
    char *fields[8];
    int sizes[8];
    int total_size;
} ComplexMessage;

/* Helper to allocate the 8 fields */
/* __attribute__((unused)) suppresses warnings if this function isn't called */
static void __attribute__((unused)) allocate_complex_message(ComplexMessage *msg, int total_size) {
    msg->total_size = total_size;
    int chunk_size = total_size / 8;
    int remainder = total_size % 8;

    for (int i = 0; i < 8; i++) {
        int current_size = chunk_size + (i == 7 ? remainder : 0);
        msg->sizes[i] = current_size;
        msg->fields[i] = (char *)malloc(current_size); 
        if (!msg->fields[i]) {
            perror("Malloc failed");
            exit(EXIT_FAILURE);
        }
        memset(msg->fields[i], 'A' + i, current_size);
    }
}

/* Helper to free the 8 fields */
static void __attribute__((unused)) free_complex_message(ComplexMessage *msg) {
    for (int i = 0; i < 8; i++) {
        free(msg->fields[i]);
    }
}

/* Helper to read exactly n bytes */
static ssize_t __attribute__((unused)) read_n(int fd, void *ptr, size_t n) {
    size_t nleft = n;
    ssize_t nread;
    char *p = (char *)ptr;

    while (nleft > 0) {
        if ((nread = read(fd, p, nleft)) < 0) {
            if (errno == EINTR) nread = 0;
            else return -1;
        } else if (nread == 0) {
            break; // EOF
        }
        nleft -= nread;
        p += nread;
    }
    return (n - nleft);
}

#endif