#include "win.h"

int
init_sock(void) {
  WSADATA wsa_data;
  return WSAStartup(MAKEWORD(1, 1), &wsa_data);
}

int
quit_sock(void) {
  return WSACleanup();
}

int
close_sock(sock_t sock) {
  if (shutdown(sock, SD_BOTH)) return 1;
  return closesocket(sock);
}
