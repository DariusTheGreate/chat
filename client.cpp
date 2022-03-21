#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

#include "queryTypes.h"
#include "socketWrapper.h"
#include "network.h"
#include "query.h"
#include "response.h"

class Client{
public:
    Client(char *serverIp, int port) : serverIp(serverIp), port(port), client(AF_INET, SOCK_STREAM){
        //client = init_client_socket(serverIp, port);
        //struct hostent* host = gethostbyname(serverIp); 
        //struct sockaddr_in sendSockAddr;    
        //bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
        //sendSockAddr.sin_family = AF_INET; 
        //sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
        //sendSockAddr.sin_port = htons(port);

        client.socketConnect(serverIp, port);    
    }

    void session(){
        while(1)
        {
            std::cout << ">";
            std::string data;
            getline(std::cin, data);
            std::cout << "data size is " << data.size() << " ";
            query cq = data; 
            std::cout << "cq size is " << cq.getSize() << "\n";

            auto query = cq.getQueryType(); 

            if(query == queryType::Exit)
            {
                client.socketSend(cq.getAcess(), cq.getSize());
                break;
            }

            client.socketSend(cq.getAcess(), cq.getSize());
            
            response resp;
            client.socketRecv(resp.getAcess(), maxServerResponseSize);
            
            processClientQuery(query, resp);

            std::cout << "Server: " << resp.getAcess() << std::endl;
            resp.clearState();
        }
        
        close(client);
    }

    void processClientQuery(const queryType& q, const response& resp){
        //std::cout << "process response is: " << resp.getAcess() << "\n";
        if(resp.checkResponseType() == responseType::Negative)
            return;

        if(q == queryType::Login || q == queryType::Registration){
            loggedIn = true; 
            std::cout << "You`re successfully logged in\n";
        }


    }

private:
    char *serverIp;
    int port;
    bool loggedIn = false;
    socketWrapper client;
};

int main(int argc, char *argv[])
{
    //we need 2 things: ip address and port number, in that order
    if(argc != 3)
    {
        std::cerr << "Usage: ip_address port" << std::endl; exit(0); 
    }

    char *serverIp = argv[1]; int port = atoi(argv[2]); 
    
    Client c(serverIp, port);
    c.session();
    return 0;    
}
