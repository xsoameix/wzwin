#include "unix.h"

int init_sock(void) { return 0; }
int quit_sock(void) { return 0; }

int
close_sock(sock_t sock) {
  if (shutdown(sock, SHUT_RDWR)) return 1;
  return close(sock);
}
