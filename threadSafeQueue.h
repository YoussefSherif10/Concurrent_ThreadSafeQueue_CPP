//
// Created by youssef fekry on 11/02/2023.
//

#ifndef THREADSAFEQUEUE_THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_THREADSAFEQUEUE_H

#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
private:
    mutable std::mutex m; // to be accessed by const methods
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    ThreadSafeQueue(){}

    /**
     * copy constructor. we need to lock the mutex of the other queue
     * then, copy the data queue
     * @param other : different object of the same class to be copied.
     */
    ThreadSafeQueue(const ThreadSafeQueue &other){
        std::lock_guard<std::mutex> guard(other.m);
        data_queue = other.data_queue;
    }

    // disable the assignment operator
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    /**
     * To add a new element ot queue, first, lock mutex then, add element
     * then, notify it is done
     * @param new_value : new element to be added to the queue
     */
    void push(T new_value){
        std::scoped_lock guard(m);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    /**
     * wait for the condition variable then, update a referenced parameter
     * then, remove the element
     * NOTE: the integration of front and pop to avoid race condition.
     * @param value : ref parameter to be updated with new value.
     */
    void wait_and_pop(T &value){
        std::unique_lock<std::mutex> guard(m);
        // the predicate lambda expression defines the condition that we wait for
        data_cond.wait(guard, [this]{return !data_queue.empty();});
        value = data_queue.front();
        data_queue.pop();
    }

    /**
     * An overload to method that waits for the condition variable, then get
     * the front element of the queue then, remove the element for the queue.
     * NOTE : the integration of front and pop to avoid race condition.
     * @return shared pointer to the front element.
     */
    std::shared_ptr<T> wait_and_pop(){
        std::unique_lock<std::mutex> guard(m);
        data_cond.wait(guard, [this]{return !data_queue.empty();});
        std::shared_ptr<T> ptr(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return ptr;
    }

    /**
     * this method tries to get element but with no waiting
     * in case it fails, it returns false. otherwise, it updates the ref parameter
     * with the new value then, removes it from the queue
     * @param value : ref parameter to be updated with the new value
     * @return the state of retrieving element.
     */
    bool try_pop(T &value){
        std::lock_guard<std::mutex> guard(m);
        if (data_queue.empty())
            return false;
        value = data_queue.front();
        data_queue.pop();
        return true;
    }

    /**
     * An overload that returns null pointer in case of failure with no waiting
     * otherwise, it returns pointer to the retrieved element.
     * @return pointer either null or with the value.
     */
    std::shared_ptr<T> try_pop(){
        std::lock_guard<std::mutex> guard(m);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> ptr(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return ptr;
    }

    /**
     * checks the emptiness of the queue safely (after locking the mutex)
     * @return the state of the queue according to its size
     */
    bool empty() const {
        std::lock_guard<std::mutex> guard(m);
        return data_queue.size();
    };
};

#endif //THREADSAFEQUEUE_THREADSAFEQUEUE_H
