#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "request_handler.h"

class ThreadPool{
private:
    std::vector<std::thread> threads;
    std::queue<int> clientQueue;
    std::condition_variable condition;
    std::mutex mutex;
    RequestHandler requestHandler;
public:
    ThreadPool(int numberThreads);
    void enqueue(int clientFD);
};

#endif