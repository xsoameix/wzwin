#ifndef NET_WIN_H
#define NET_WIN_H

#pragma warning(push)
#pragma warning(disable: 4255) // func() => func(void)
#pragma warning(disable: 4574) // #define xxx 0
#pragma warning(disable: 4668) // xxx is not defined as macro
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma warning(pop)

#define NET_INVALID_SOCK INVALID_SOCKET

typedef SOCKET sock_t;
typedef int ssize_t;

#endif
