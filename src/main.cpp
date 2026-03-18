#include "server.h"

/*
Multithreaded HTTP Web Server
Built from scratch using raw POSIX sockets and C++17

architecture:
    Listener   -> creates socket, binds to port, accept loop
    ThreadPool -> N worker threads, shared queue, mutex + condition variable
    Handler    -> reads raw bytes, parses HTTP, serves files, sends response

usage:
    run from project root: ./build/webserver
    open browser at: http://localhost:8080
*/

int main() {
    /*
    port 8080 -> no root permissions required (port 80 requires root)
    8 threads -> one per logical core, handles 8 clients concurrently
    */
    Server server(8080, 16);
    server.start();

    return 0;
}