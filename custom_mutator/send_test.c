#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#define PORT 1234
#define SERVER_IP "127.0.0.1"


void fuzz_send(uint8_t *buf, size_t buf_size) {

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  printf("sockfd %d",sockfd);
  if (sockfd < 0) {
    printf("Socket creation failed\n");
    return;
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

  //sendto(sockfd, buf, buf_size, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  printf("Data sent successfully\n");

  close(sockfd);

  return;
}

int main() {
  // Example data to send
  uint8_t data[] = "Hello, this is a test message.";
  size_t data_size = sizeof(data);

  printf("test!");
  fuzz_send(data, data_size);

  return 0;
}
