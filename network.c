#include "network.h"
#include <netdb.h>

#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <stddef.h>
#include <stdlib.h>

int8_t parse_address(char* adress, char* ip_dest, char* port_dest);

int8_t init_server_socket(int port){
    struct sockaddr_in addr;  
    bzero((void*)&addr, sizeof(addr));
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
        
    if(serverSd < 0)
    {
        return -1;
    }

    int bindStatus = bind(serverSd, (struct sockaddr*) &addr, sizeof(addr));
    if(bindStatus < 0)
    {
        return -1; 
    }

    return serverSd; 
}

int accept_server_client(int server, int n){
    listen(server, n);

    struct sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    int newSd = accept(server, (struct sockaddr*)&newSockAddr, &newSockAddrSize);        
    
    if(newSd < 0)
    {
       return -1; 
    }
    
    return newSd;
}

int init_client_socket(char* serverIp, int port){
    struct hostent* host = gethostbyname(serverIp); 
    struct sockaddr_in sendSockAddr;    
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);

    int clientSd = socket(AF_INET, SOCK_STREAM, 0);

    int status = connect(clientSd, (struct sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        return -1;
    }

    return clientSd;
}

int close_net(int server){
    return close(server);
}
