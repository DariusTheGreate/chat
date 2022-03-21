#ifndef _NET_
#define _NET_

#include <stddef.h>
#include <stdint.h>

int init_client_socket(char* serverIp, int port);
int accept_server_client(int server, int n);
int8_t init_server_socket(int port);
int close_net(int server);

#endif
