#ifndef _NET_
#define _NET_

#include <stddef.h>
#include <stdint.h>

int8_t listen_net(char* address);
int8_t accept_net(int8_t guy);
int8_t connect_net(char* address);
int8_t close_net(int8_t guy);
int8_t send_net(int8_t gut, char* buffer, uint32_t size);
int8_t recv_net(int8_t guy, char* buffer, uint32_t size);
int8_t parse_address(char *address, char *ip, char *port);

#endif
