/*
my server class will take two things for now
a constrctor -> takes a port numbr and sets server up
a start method that begins listening for connections 
private members sever_fd-> the socket file desrciptor
*/

#ifndef SERVER_H
#define SERVER_H

#include "thread_pool.h"

class Server{
private:
    int serverFD;
    int port;
    ThreadPool threadPool;

public:
    Server(int port, int numThreads);
    void start();
};
#endif