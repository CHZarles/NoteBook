#include "utils.hpp"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
  int sock;
  char message[BUF_SIZE];
  int str_len;
  socklen_t adr_sz;
  struct sockaddr_in serv_adr, from_adr;
  if (argc != 3) {
    printf("Usage: %s <IP> <port>\n", argv[0]);
    exit(1);
  }
  // create udp socket
  sock = socket(PF_INET, SOCK_DGRAM, 0);
  CHECK_ERROR(sock, "socket() error");

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_adr.sin_port = htons(atoi(argv[2]));

  while (1) {
    fputs("Insert  messgae(q to quit): ", stdout);
    fgets(message, sizeof(message), stdin);
    if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
      break;
    sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_adr,
           sizeof(serv_adr));
    adr_sz = sizeof(from_adr);
    str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&from_adr,
                       &adr_sz);
    message[str_len] = 0;
    printf("Message from server : %s", message);
  }
  close(sock);
  return 0;
}
