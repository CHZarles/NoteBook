#include "utils.hpp"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
  int sock;
  char message[BUF_SIZE];
  int str_len;
  socklen_t adr_sz; // 多余变量

  struct sockaddr_in serv_adr, from_adr; // 不需要 from_adr
  if (argc != 3) {
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(1);
  }
  // 创建 UDP 套接字
  sock = socket(PF_INET, SOCK_DGRAM, 0);
  CHECK_ERROR(sock, "socket() error");

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_adr.sin_port = htons(atoi(argv[2]));

  connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));

  while (1) {
    fputs("Insert message(q to quit): ", stdout);
    fgets(message, sizeof(message), stdin);
    if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
      break;
    // 向服务器传输数据,会自动给自己分配IP地址和端口号

    /*
    sendto(sock, message, strlen(message), 0,
           (struct sockaddr *)&serv_adr, sizeof(serv_adr));
    */
    write(sock, message, strlen(message));
    /*
    adr_sz = sizeof(from_adr);
    str_len = recvfrom(sock, message, BUF_SIZE, 0,
                       (struct sockaddr *)&from_adr, &adr_sz);
    */
    str_len = read(sock, message, sizeof(message) - 1);
    message[str_len] = 0;
    printf("Message from server: %s", message);
  }
  close(sock);
  return 0;
}
