#include <iostream>
#include <string>
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

#include "thread_pool/ThreadPool.h"

using namespace std;

class Server{
public:
    Server(int port) : clients(5){
        bzero((char*)&servAddr, sizeof(servAddr));

        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servAddr.sin_port = htons(port);


        serverSd = socket(AF_INET, SOCK_STREAM, 0);

        if(serverSd < 0)
        {
            cerr << "Error establishing the server socket" << endl;
            exit(0);
        }
    }

    void bindme(){
        int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr));
        if(bindStatus < 0)
        {
            cerr << "Error binding socket to local address" << endl;
            exit(0);
        }
    }

    void connect(){
        while(1){
            cout << "Waiting for a client to connect..." << endl;
            listen(serverSd, 5);
            sockaddr_in newSockAddr;
            socklen_t newSockAddrSize = sizeof(newSockAddr);
            int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
        
            if(newSd < 0)
            {
                cerr << "Error accepting request from client!" << endl;
                exit(1);
            }

            clients.Submit([this, newSd]() {this->session(serverSd, newSd);});
            cout << "Connected with client!" << endl;
        }
    }

    void session(int serverSd, int newSd){
        struct timeval start1, end1;
        gettimeofday(&start1, NULL);
        char msg[1000];
        
        while(1)
        {
            cout << "Awaiting client response..." << endl;
            memset(&msg, 0, sizeof(msg));//clear the buffer
            recv(newSd, (char*)&msg, sizeof(msg), 0);

            if(!strcmp(msg, "exit"))
            {
                cout << "Client has quit the session" << endl;
                break;
            }

            cout << "Client: " << msg << endl;
            cout << ">";
            string data;
            getline(cin, data);
            memset(&msg, 0, sizeof(msg)); //clear the buffer
            strcpy(msg, data.c_str());

            if(data == "exit")
            {
                send(newSd, (char*)&msg, strlen(msg), 0);
                break;
            }
            
            send(newSd, (char*)&msg, strlen(msg), 0);            

        }
        
        gettimeofday(&end1, NULL);
        close(newSd);
        close(serverSd);
    }

private:
    sockaddr_in servAddr;
    ThreadPool clients;
    int serverSd;
};

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cerr << "Usage: port" << endl;
        exit(0);
    }

    int port = atoi(argv[1]);
    Server s(port);

    s.bindme();
    s.connect();
    //s.session();
    return 0;   
}
