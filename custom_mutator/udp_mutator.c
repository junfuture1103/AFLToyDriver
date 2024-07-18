#include "afl-fuzz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 1234
#define SERVER_IP "127.0.0.1"

typedef struct {
    int sockfd;
    struct sockaddr_in servaddr;
} udp_mutator_t;

void *afl_custom_init(void *afl, unsigned int seed) {
    udp_mutator_t *mutator = (udp_mutator_t *)malloc(sizeof(udp_mutator_t));
    if (!mutator) return NULL;

    if ((mutator->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        free(mutator);
        return NULL;
    }

    memset(&mutator->servaddr, 0, sizeof(mutator->servaddr));
    mutator->servaddr.sin_family = AF_INET;
    mutator->servaddr.sin_port = htons(PORT);
    mutator->servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    return mutator;
}

size_t afl_custom_fuzz(void *data, unsigned char *buf, size_t buf_size, unsigned char **out_buf, size_t *out_buf_size, unsigned char *add_buf, size_t add_buf_size) {
    udp_mutator_t *mutator = (udp_mutator_t *)data;

    sendto(mutator->sockfd, buf, buf_size, 0, (const struct sockaddr *)&mutator->servaddr, sizeof(mutator->servaddr));

    *out_buf = buf;
    *out_buf_size = buf_size;
    return buf_size;
}

void afl_custom_deinit(void *data) {
    udp_mutator_t *mutator = (udp_mutator_t *)data;
    close(mutator->sockfd);
    free(mutator);
}
