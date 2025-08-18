#ifndef DEFS_H
#define DEFS_H

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <ws2tcpip.h>
typedef int socklen_t;
#else
typedef int SOCKET;
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif
#endif
