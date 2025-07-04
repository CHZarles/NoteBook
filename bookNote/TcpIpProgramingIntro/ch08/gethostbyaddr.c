#include "utils.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int i;
  struct hostent *host;
  struct sockaddr_in addr;
  if (argc != 2) {
    printf("Usage: %s <IP address>\n", argv[0]);
    exit(1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  host = gethostbyaddr((const void *)&addr.sin_addr, sizeof(addr.sin_addr),
                       AF_INET);
  if (!host) {
    error_handling("gethostbyaddr() error");
    exit(1);
  }
  printf("Official name: %s \n", host->h_name);
  for (i = 0; host->h_aliases[i]; i++)
    printf("Aliases %d:%s \n", i + 1, host->h_aliases[i]);
  printf("Address type: %s \n",
         (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");

  for (i = 0; host->h_addr_list[i]; i++)
    printf("IP addr %d: %s \n", i + 1,
           inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
  return 0;
}
