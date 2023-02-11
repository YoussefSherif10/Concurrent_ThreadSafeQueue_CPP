# Concurrent_ThreadSafeQueue_CPP
It is typical practise to use a Queue to move data across threads. If done correctly, the synchronisation can be kept within the queue, which drastically minimises the likelihood of synchronisation issues and race conditions.

In order to move data between threads, I created a generic templated class for this project.

In order to avoid using two global variables in the main program, I combined the mutex and variable condition with the protect and sync functions.

Three categories of methods make up the class: those that query the queue's state (size, empty), those that query its components (front, back), and those that change the queue (push, try_pop, wait_and_pop).

To get around the race condition issue, I combined the element query and update methods.

Finally, I created a tiny main application to show how to use the class.