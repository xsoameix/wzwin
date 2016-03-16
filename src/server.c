#include <stdio.h>
#include <stdlib.h>
#include "net/general.h"

void *
get_in_addr(struct sockaddr * addr) {
  if (addr->sa_family == AF_INET)
    return &((struct sockaddr_in *) addr)->sin_addr;
  else
    return &((struct sockaddr_in6 *) addr)->sin6_addr;
}

int
main(int argc, char ** argv) {
  int ret = 1;
  if (init_sock()) goto init_sock_fail;
  const char * local = "127.0.0.1";
  if (argc >= 2) local = argv[1];
  const char * port = "5400";
  if (argc >= 3) port = argv[2];
  struct addrinfo hint = {0}, * info;
  hint.ai_family = AF_UNSPEC;
  hint.ai_socktype = SOCK_DGRAM;
  hint.ai_flags = AI_PASSIVE;
  int err;
  if ((err = getaddrinfo(local, port, &hint, &info)) != 0) {
    fprintf(stderr, "%s\n", gai_strerror(err)); goto getaddrinfo_fail;
  }
  sock_t sock = NET_INVALID_SOCK;
  struct addrinfo * p = info;
  for (; p != NULL; p = p->ai_next) {
    if ((sock = socket(p->ai_family, p->ai_socktype,
                       p->ai_protocol)) == NET_INVALID_SOCK) {
      perror("socket"); continue;
    }
    if (bind(sock, p->ai_addr, p->ai_addrlen) == -1) {
      perror("bind"), close_sock(sock); continue;
    }
    break;
  }
  freeaddrinfo(info);
  if (p == NULL) {
    fprintf(stderr, "failed to bind socket\n"); goto socket_fail;
  }
  struct sockaddr_storage addr_storage;
  struct sockaddr * addr = (struct sockaddr *) &addr_storage;
  socklen_t addr_len = sizeof(addr_storage);
  size_t size = 65535;
  char * msg = malloc(size);
  if (msg == NULL) goto malloc_fail;
  ssize_t len;
  if ((len = recvfrom(sock, msg, size, 0, addr, &addr_len)) == -1) {
    perror("recvfrom");
  }
  msg[len] = '\0';
  char ip[INET6_ADDRSTRLEN];
  printf("packet from %s\n",
         inet_ntop(addr->sa_family, get_in_addr(addr), ip, sizeof(ip)));
  printf("packet: %s\n", msg);
  free(msg);
  close_sock(sock);
  ret = 0;
malloc_fail:
socket_fail:
getaddrinfo_fail: quit_sock();
init_sock_fail:
  return ret;
}
