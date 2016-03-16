#ifndef NET_GENERAL_H
#define NET_GENERAL_H

#ifdef _WIN32
#  include "win.h"
#else
#  include "unix.h"
#endif

int close_sock(sock_t sock);
int init_sock(void);
int quit_sock(void);

#endif
