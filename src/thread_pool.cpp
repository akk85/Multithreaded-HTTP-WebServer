#include "thread_pool.h"
#include <thread>
#include <iostream>


ThreadPool::ThreadPool(int numberThreads){

    /*
    Spin up N worker threads at startup -> each thread runs a forever loop waiting for work -> threads are idle until a client fd arrives in the queue
    */
    for (int thread = 0; thread < numberThreads; thread ++){
        std::thread newThread([this]() {

            /*
            lock the mutex before touching the queue
            thread goes to sleep -> wakes up when enqueue() signals, re-acquires mutex automatically
            */
            while(true){
                std::unique_lock<std::mutex> lock(mutex);

                condition.wait(lock, [this] () {
                    return !clientQueue.empty(); // lambda function thread goes to sleep if queue empty else wakes up and re-acquires the mutex 
                });

                /*
                pop client FD from queue -> unlock happens automatically when lock goes out of scope (RAII)
                */
                int currentClient =  clientQueue.front(); clientQueue.pop();

                requestHandler.handleClient(currentClient);
            }

           
        });
        threads.push_back(std::move(newThread));
    }
}

/*
Listener -> enqueue called by listener to push a new client fd into the queue
Locks the mutex for duration of this function
Pushes client and signals condition variable to wake up one sleeping thread
mutex gets unlocked automatically when function goes out of scope due to RAII handles it
*/
void ThreadPool::enqueue(int clientFD){
    std::lock_guard<std::mutex> lock(mutex); 

    clientQueue.push(clientFD);

    condition.notify_one();
}