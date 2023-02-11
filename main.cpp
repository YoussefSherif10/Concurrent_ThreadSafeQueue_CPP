#include <iostream>
#include <thread>
#include "threadSafeQueue.h"

using namespace std;

// create an instance of the safe queue class to handle sync.
ThreadSafeQueue<int> data_queue;

/**
 * wait for the second thread to push data in the safe queue
 * then, prints it.
 */
void firstThread(){
    int data = 0;
    // update variable data when ready
    data_queue.wait_and_pop(data);
    // print the waited time
    cout << "First Thread waited for " << data << " milliseconds\n";
}

/**
 * pushes data to the safe queue
 */
void secondThread(){
    cout << "Started the second Thread\n";
    // wait for 10 milliseconds
    this_thread::sleep_for(chrono::milliseconds(10));
    // update the queue
    data_queue.push(10);
}

int main() {
    // start the two threads
    thread t1(firstThread);
    thread t2(secondThread);

    // wait the threads to finish execution
    t1.join();
    t2.join();

    return 0;
}
