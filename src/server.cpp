#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "server.h"

Server::Server(int port, int numThreads) : threadPool(numThreads) {
    this->port = port;

    /*
    Create Socket 
    AF_INET = address family internet tells os I want IPV4
    SOCK_STREAM = socket type ->tcp here UDP would be SOCKET_DGRAM
    returns a file descriptor (integer) the OS uses to track this socket
    */
    serverFD = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFD == -1){
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    std::cout << "Socket created successfully. file descriptor = " << serverFD << std::endl;


    
    /*
    Bind socket to port 
    zero out entire struct before filling it in to avoid garbage values
    htons -> convert port number into correct byte order that the network understands. 
    */
    sockaddr_in address;
    memset(&address, 0, sizeof(address));  
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(this->port);

    int bound = bind(serverFD, (sockaddr*)&address, sizeof(address));
    if (bound == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        return;
    }

    std::cout << "Socket bound successfully to port " << this->port << std::endl;

    /*
    Listen -> tell OS  to start queing incoming connections on this port -> backlog of 10 -> OS will hold up to 10 pending connections in the waiting room
    */
    int listening = listen(serverFD, 10);
    if (listening == -1){
        std::cerr << "Failed to listen on socket" << std::endl;
        return;
    }
    std::cout << "Listening on port " << this->port << std::endl;
}

void Server::start(){
    std::cout << "Server starting on port " << port << std::endl;

    /*
    Accept loop block on accept consuming zero CPU until a client connects  
    */
    while(true){
        sockaddr_in clientAddress;
        socklen_t   clientLength = sizeof(clientAddress);

        int clientFD = accept(serverFD, (sockaddr*)&clientAddress, &clientLength);

        if (clientFD == -1){
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        std::cout << "Client " << clientFD << " connected " << std::endl;
        threadPool.enqueue(clientFD);
    }
}