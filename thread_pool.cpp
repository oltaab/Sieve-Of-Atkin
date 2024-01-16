#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
using namespace std;

class thread_pool {
private:
    // Vector to store worker threads
    vector<thread> threads_;

    // Queue of tasks
    queue<function<void()>> tasks_;

    // Mutex to synchronize access to shared data
    mutex queue_mutex_;

    // Condition variable to signal changes in the state of the tasks queue
    condition_variable cv_;

    // Flag to indicate whether the thread pool should stop or not
    bool stop_ = false;

public:
    // Constructor to create a thread pool with given number of threads
    thread_pool(size_t num_threads = thread::hardware_concurrency())
    {
        // Creating worker threads
        for (size_t i = 0; i < num_threads; ++i) {
            threads_.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        // Locking the queue so that data can be shared safely
                        unique_lock<mutex> lock(queue_mutex_);

                        // Waiting until there is a task to execute or the pool is stopped
                        cv_.wait(lock, [this] {
                            return !tasks_.empty() || stop_;
                        });

                        // Exit the thread in case the pool is stopped and there are no tasks
                        if (stop_ && tasks_.empty()) {
                            return;
                        }

                        // Get the next task from the queue
                        task = move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                    {
                        unique_lock<mutex> lock(queue_mutex_);
                    }
                }
            });
        }
    }

    // Destructor to stop the thread pool
    ~thread_pool()
    {
        {
            // Lock the queue to update the stop flag safely
            unique_lock<mutex> lock(queue_mutex_);
            stop_ = true;
        }

        // Notify all threads
        cv_.notify_all();

        // Joining all worker threads to ensure they have completed their tasks
        for (auto &thread : threads_) {
            thread.join();
        }
    }

    // Enqueue task for execution by the thread pool
    void enqueue(function<void()> task)
    {
        {
            unique_lock<std::mutex> lock(queue_mutex_);
            tasks_.push(move(task));
        }
        cv_.notify_one();
    }

};
