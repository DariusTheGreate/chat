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

#include "http.h"

using namespace std;
//Client side

class Client{
public:
    Client(char *serverIp, int port) : serverIp(serverIp), port(port){
        struct hostent* host = gethostbyname(serverIp); 
        bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
        sendSockAddr.sin_family = AF_INET; 
        sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
        sendSockAddr.sin_port = htons(port);

        clientSd = socket(AF_INET, SOCK_STREAM, 0);
    }

    void session(){
        int status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
        if(status < 0)
        {
            cout<<"Error connecting to socket!"<<endl; 
            //break;
        }
        cout << "Connected to the server!" << endl;
        struct timeval start1, end1;
        gettimeofday(&start1, NULL);
        while(1)
        {
            cout << ">";
            string data;
            getline(cin, data);
            memset(&msg, 0, sizeof(msg));//clear the buffer
            strcpy(msg, data.c_str());
            if(data == "exit")
            {
                send(clientSd, (char*)&msg, strlen(msg), 0);
                break;
            }
            send(clientSd, (char*)&msg, strlen(msg), 0);
            cout << "Awaiting server response.. " << clientSd << endl;
            memset(&msg, 0, sizeof(msg));//clear the buffer
            recv(clientSd, (char*)&msg, sizeof(msg), 0);
            if(!strcmp(msg, "exit"))
            {
                cout << "Server has quit the session" << endl;
                break;
            }
            cout << "Server: " << msg << endl;
        }
        gettimeofday(&end1, NULL);
        close(clientSd);
    }

private:
    char *serverIp;
    int port;
    sockaddr_in sendSockAddr;   
    char msg[1500]; 
    int clientSd;
};

int main(int argc, char *argv[])
{
    //we need 2 things: ip address and port number, in that order
    if(argc != 3)
    {
        cerr << "Usage: ip_address port" << endl; exit(0); 
    } //grab the IP address and port number 
    char *serverIp = argv[1]; int port = atoi(argv[2]); 
    //create a message buffer 
    
    Client c(serverIp, port);
    c.session();
    return 0;    
}
