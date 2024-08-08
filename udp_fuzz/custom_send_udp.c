#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "afl-fuzz.h"

static int my_debug = 0;
static int my_read = 0;

#define DEBUG(...) if (my_debug) printf(__VA_ARGS__)

typedef struct udp_send_mutator {
    afl_state_t* afl;
    struct sockaddr_in server_addr;
} udp_send_mutator_t;

void send_file_data(const char *buffer, const char *dest_ip, int dest_port) {
    printf("==== send data! in custom mutator : %s ====\n", buffer);
    int sockfd;
    struct sockaddr_in destaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&destaddr, 0, sizeof(destaddr));
    destaddr.sin_family = AF_INET;
    destaddr.sin_port = htons(dest_port);

    if (inet_pton(AF_INET, dest_ip, &destaddr.sin_addr) <= 0) {
        perror("invalid address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&destaddr, sizeof(destaddr));
    close(sockfd);
}

void *afl_custom_init(afl_state_t* afl, uint32_t seed) {
    const char* ip = getenv("CUSTOM_SEND_IP");
    const char* port = getenv("CUSTOM_SEND_PORT");

    if (getenv("AFL_DEBUG")) my_debug = 1;
    if (getenv("CUSTOM_SEND_READ")) my_read = 1;

    if (!ip || !port) {
       fprintf(stderr, "You forgot to set CUSTOM_SEND_IP and/or CUSTOM_SEND_PORT\n");
       exit(1); 
    }

    udp_send_mutator_t* mutator = calloc(1, sizeof(udp_send_mutator_t));
    if (!mutator) {
       fprintf(stderr, "Failed to allocate mutator struct\n");
       exit(1); 
    }

    mutator->afl = afl;

    bzero(&mutator->server_addr, sizeof(mutator->server_addr));
    mutator->server_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip, &mutator->server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Could not convert target ip address!\n");
        exit(1);
    }
    mutator->server_addr.sin_port = htons(atoi(port));
    
    printf("[+] Custom udp send mutator setup ready to go!\n");

    return mutator;
}
// int try_connect(udp_send_mutator_t *mutator, int sock, int max_attempts) {
int try_connect(int max_attempts) {
    while (max_attempts > 0) {
        // if (connect(sock, (struct sockaddr*)&mutator->server_addr, sizeof(mutator->server_addr)) == 0) {
        //     return 0;
        // }

        // Even with AFL_CUSTOM_LATE_SEND=1, there is a race between the
        // application under test having started to listen for connections and
        // afl_custom_fuzz_send being called. To address this race, we attempt
        // to connect N times and sleep a short period of time in between
        // connection attempts.
        struct timespec t;
        t.tv_sec = 6;
        t.tv_nsec = 0;
        nanosleep(&t, NULL);
        --max_attempts;

        printf("=============== send custom udp! =============== %d\n", max_attempts);
    }
    return 1;
}
void afl_custom_fuzz_send(udp_send_mutator_t *mutator, uint8_t *buf, size_t buf_size) {
    
    try_connect(1);
    send_file_data(buf, "127.0.0.1", 1234);
}

void afl_custom_deinit(udp_send_mutator_t* mutator) {
    free(mutator);
}
