#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 12345

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

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Cannot open input file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *data = (uint8_t *)malloc(size);
    if (!data) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    fread(data, 1, size, file);
    fclose(file);

    LLVMFuzzerTestOneInput(data, size);
    free(data);

    return 0;
}
