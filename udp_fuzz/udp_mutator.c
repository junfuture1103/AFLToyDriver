
//
// This is an example on how to use afl_custom_send
// It writes each mutated data set to /tmp/foo
// You can modify this to send to IPC, shared memory, etc.
//
// cc -O3 -fPIC -shared -g -o custom_send.so -I../../include custom_send.c
// cd ../..
// afl-cc -o test-instr test-instr.c
// AFL_CUSTOM_MUTATOR_LIBRARY=custom_mutators/examples/custom_send.so \
//   afl-fuzz -i in -o out -- ./test-instr -f /tmp/foo
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "afl-fuzz.h"

#define PORT 1234
#define SERVER_IP "127.0.0.1"

typedef struct my_mutator {

  afl_state_t *afl;

} my_mutator_t;

my_mutator_t *afl_custom_init(afl_state_t *afl, unsigned int seed) {

  my_mutator_t *data = calloc(1, sizeof(my_mutator_t));
  if (!data) {

    perror("afl_custom_init alloc");
    return NULL;

  }

  data->afl = afl;

  return data;

}

void afl_custom_fuzz_send(my_mutator_t *data, uint8_t *buf, size_t buf_size) {

    int sockfd;
    struct sockaddr_in server_addr;

    // 소켓 생성
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return;
    }

    // 메시지 전송
    sendto(sockfd, buf, buf_size, 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("============ Message sent by driver : %s ============ \n", buf);
    printf("addr : %s port : %d\n", SERVER_IP, PORT);

    close(sockfd);
    return;
}



void afl_custom_deinit(my_mutator_t *data) {

  free(data);

}