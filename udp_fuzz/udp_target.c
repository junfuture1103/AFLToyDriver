#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void process_input(const char *input) {
    printf("process input : %s\n", input);

    if (strncmp(input, "FUZZ", 3) == 0) {
        printf("Detected FUZZ prefix\n");

        if (strstr(input, "FUZZ_C") != NULL) {
            printf("CRASH found in input, triggering error...\n");
            abort(); // This will cause a crash for testing
        }

        if (strcmp(input, "FUZZ_AFL") == 0) {
            printf("Exact match for FUZZ_AFL\n");
        } else if (strcmp(input, "FUZZ_TEST") == 0) {
            printf("Exact match for FUZZ_TEST\n");
        } else {
            printf("FUZZ prefix but not an exact match\n");
        }
    } else {
        printf("No FUZZ prefix\n");
    }

}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    socklen_t len = sizeof(cliaddr);

    printf("hello! server on!!\n");

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(1234);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&cliaddr, &len);
        if (n > 0) {
            buffer[n] = '\0';
            //__AFL_COVERAGE_ON();
            process_input(buffer);
            //__AFL_COVERAGE_OFF();
        }
    }

    close(sockfd);
    return 0;
}
