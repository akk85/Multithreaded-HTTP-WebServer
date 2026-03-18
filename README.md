# Multithreaded HTTP Web Server

HTTP/1.1 web server built from scratch in C++ — raw POSIX sockets, custom thread pool, mutex + condition variable synchronization. No frameworks.

## Benchmark
`wrk -t4 -c100 -d10s http://localhost:8080`
```
Requests/sec:  5,503
Avg latency:   1.77ms
Transferred:   88.32MB in 10s
```

## Architecture
![Architecture](architecture.png)

- **Listener** — TCP socket, binds to port 8080, blocks on `accept()`, hands client fds to thread pool
- **Thread Pool** — N worker threads sleeping on a condition variable, wake on new connections, mutex-protected queue
- **Request Handler** — parses HTTP request line, serves files from `static/`, returns 404 if not found

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