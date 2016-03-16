#include <stdio.h>
#include <string.h>
#include "net/general.h"

int
main(int argc, char ** argv) {
  int ret = 1;
  if (init_sock()) goto init_sock_fail;
  const char * remote = "127.0.0.1";
  if (argc >= 2) remote = argv[1];
  const char * port = "5400";
  if (argc >= 3) port = argv[2];
  struct addrinfo hint = {0}, * info;
  hint.ai_family = AF_UNSPEC;
  hint.ai_socktype = SOCK_DGRAM;
  int err;
  if ((err = getaddrinfo(remote, port, &hint, &info)) != 0) {
    fprintf(stderr, "%s\n", gai_strerror(err)); goto getaddrinfo_fail;
  }
  struct addrinfo * p = info;
  sock_t sock;
  for (; p != NULL; p = p->ai_next) {
    if ((sock = socket(p->ai_family, p->ai_socktype,
                       p->ai_protocol)) == NET_INVALID_SOCK) {
      perror("socket"); continue;
    }
    break;
  }
  if (p == NULL) {
    fprintf(stderr, "failed to bind socket\n"); goto socket_fail;
  }
  char msg[] = "hello world";
  ssize_t bytes;
  if ((bytes = sendto(sock, msg, strlen(msg), 0,
                      p->ai_addr, p->ai_addrlen)) == -1) {
    perror("sendto\n");
  }
  close_sock(sock);
  ret = 0;
socket_fail: freeaddrinfo(info);
getaddrinfo_fail: quit_sock();
init_sock_fail:
  return ret;
}
