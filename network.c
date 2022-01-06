#include "network.h"

#include <unistd.h>

#include <arpa/inet.h>

#include <stddef.h>
#include <stdlib.h>

int8_t parse_address(char* adress, char* ip_dest, char* port_dest);

int8_t listen_net(char* address){
	int32_t sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sock < 0){
		return -1;
	}

	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, NULL, sizeof(int)) < 0){
		return -1;
	}

	char ip[16];
	char port[6];
	
	if(parse_address(address, ip, port)){
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);

	if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0){
		return -1;
	}

	if(listen(sock, SOMAXCONN) != 0){
		return -1;
	}

	return sock;
}

int8_t connect_net(char *address) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sock < 0) {
		return -1;
	}

	char ip[16];
	char port[6];
	
	if (parse_address(address, ip, port) != 0) {
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);
	
	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
		return -1;
	}

	return sock;
}

int8_t close_net(int8_t guy){
	return close(guy);
}

int8_t send_net(int8_t guy, char* buffer, uint32_t size){
	return send(guy, buffer, (int)size, 0);
}

int8_t recv_net(int8_t guy, char* buffer, uint32_t size){
	return recv(guy, buffer, (int)size, 0);
}

int8_t parse_address(char *address, char *ip, char *port) {
	size_t i = 0, j = 0;
	for (; address[i] != ':'; ++i) {
		if (address[i] == '\0') {
			return 1;
		}
		if (i >= 15) {
			return 2;
		}
		ip[i] = address[i];
	}
	ip[i] = '\0';
	for (i += 1; address[i] != '\0'; ++i, ++j) {
		if (j >= 5) {
			return 3;
		}
		port[j] = address[i];
	}
	port[j] = '\0';
	return 0;
}
