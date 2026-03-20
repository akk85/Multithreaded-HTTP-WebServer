# Multithreaded HTTP Web Server

HTTP/1.1 web server built from scratch in C++ — raw POSIX sockets, custom thread pool, mutex + condition variable synchronization. No frameworks.

Benchmarked at **5,500+ requests/sec** under 100 concurrent connections with **1.77ms average latency**.

**[Full writeup, architecture, and challenges → tonykariuki.com/projects/webserver](https://tonykariuki.com/markdown/projects/webserver)**

## Build & Run
```bash
git clone https://github.com/akk85/Multithreaded-HTTP-WebServer.git
cd Multithreaded-HTTP-WebServer
mkdir build && cd build
cmake .. && make
cd .. && ./build/webserver
```
Open `http://localhost:8080`

## Stack
C++17 · POSIX Sockets · pthreads · CMake
