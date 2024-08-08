#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 1234

void send_udp_packet(const char *data, size_t size) {
    int sockfd;
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(TARGET_PORT);
    servaddr.sin_addr.s_addr = inet_addr(TARGET_IP);

    sendto(sockfd, data, size, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    close(sockfd);
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    send_udp_packet((const char *)Data, Size);
    return 0;
}

void generate_random_string(char *str, size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    if (size) {
        for (size_t n = 0; n < size - 1; n++) {
            int key = rand() % (int)(sizeof(charset) - 1);
            str[n] = charset[key];
        }
        str[size - 1] = '\0';
    }
}

int main() {
    srand(time(NULL));

    size_t size = 8;
    char data[size + 1]; // 8자리 문자열 + NULL 종료 문자
    generate_random_string(data, size + 1);

    LLVMFuzzerTestOneInput((const uint8_t *)data, size);
    
    return 0;
}
