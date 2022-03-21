#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#include "queryTypes.h"
#include "network.h"
#include "responseTypes.h"
#include "thread_pool/ThreadPool.h"
#include "response.h"
#include "query.h"
#include "socketWrapper.h"
#include "DB.h"

class Server{
public:
    Server(int port) : m_clients(5), m_server(AF_INET, SOCK_STREAM), dbNames("usersListik.txt"){
        m_server.socketBind(port);
    }

    void connect(){
        while(1){
            std::cout << "Waiting for a client to connect..." << std::endl;
            
            m_server.socketListen(5); 
            int newSd = m_server.socketAccept();
            m_clients.Submit([this, &newSd]() {this->session(newSd);});
            std::cout << "Connected with client!" << std::endl;
            unsigned int arr[5] = { 0x05, 0x04, 0xAA, 0x0F, 0x0D };
        }
    }

    void session(socketWrapper newSd){
        while(1)
        {
            query q;
            newSd.socketRecv(q.getAcess(), maxClientMessageSize);
            std::cout << "client number " << newSd << " send mess: " << q.getAcess() << "\n";
            response r = dispath_response(q);
             
            if(!strcmp(q.getAcess(), "exit"))
                break;
            
            newSd.socketSend(r.getAcess(), maxClientMessageSize);
            q.clearText();
        }

        close(newSd);
    }

    response dispath_response(const query& q){
        queryType qt = q.getQueryType();
        response r;
        r.clearState();
        if(qt == queryType::Login){
            processLogin(q, r);
            r.getAcess()[0] = 'P';
        }

        return r;
    }

    void processLogin(const query& q, response& r){
        auto tokens = q.getTokens();
        std::cout << tokens.size(); 

        if(tokens.size() < 2)
            r.setResponseType(responseType::Negative);
        else{
            dbNames.write(std::string(tokens[1] + " " + tokens[2]).c_str()); 
        }
    }

private:
    DB dbNames;
    ThreadPool m_clients;
    sockaddr_in servAddr;
    socketWrapper m_server;
};

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: port" << std::endl;
        exit(0);
    }

    int port = atoi(argv[1]);
    Server s(port);

    s.connect();
    return 0;   
}

