#ifndef _SOCKET_WRAPPER_
#define _SOCKET_WRAPPER_

#include <exception>
#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "network.h"

class socketWrapper{ 
public: 
    socketWrapper(int domain, int type, int protocol = 0){
        serverSd = socket(domain, type, protocol);
        if(serverSd < 0)
            throw std::exception();
    }

    socketWrapper(int sock) : serverSd(sock) {}

    void socketBind(int port){
        sockaddr_in addr;
        bzero((void*)&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port);
       
        int binding = bind(serverSd, (sockaddr*) &addr, sizeof(addr)); 
        if(binding < 0)
            throw std::exception();
    }
    
    void socketListen(int n){
        listen(serverSd, n);
    }

    void socketSend(char* mes, int size, int flags = 0){
        send(serverSd, mes, size, flags);
    }
    
    void socketRecv(char* mes, int size, int flags = 0){
        recv(serverSd, mes, size, flags);
    }

    int socketAccept(){
        struct sockaddr_in newSockAddr;
        socklen_t newSockAddrSize = sizeof(newSockAddr);

        int newSd = accept(serverSd, (sockaddr*)&newSockAddr, &newSockAddrSize);        
        
        if(newSd < 0)
        {
            throw std::exception();
        }

        return (newSd);
    }

    void socketConnect(char* serverIp, int port){
        struct hostent* host = gethostbyname(serverIp); 
        struct sockaddr_in sendSockAddr;    
        bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
        sendSockAddr.sin_family = AF_INET; 
        sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
        sendSockAddr.sin_port = htons(port);


        int status = connect(serverSd, (sockaddr*)&sendSockAddr, sizeof(*(sockaddr*)&sendSockAddr));


        if(status < 0)
            throw std::exception();
    }

    operator int(){
        return serverSd;
    }

    ~socketWrapper(){
        close_net(serverSd);
    }

private:
    int serverSd;
};

#endif

