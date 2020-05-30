#include "executor.h"

Executor::Executor(int threadCount) {
    this->closed = false;

    for ( int i = 0 ; i < threadCount ; i++ ) {
        this->threads.emplace_back(&Executor::runThread, this);
    }
}

void Executor::runThread() {
    while ( !closed ) {
        Task task;

        {
            std::unique_lock<std::mutex> lock(mutex);

            if (pendingTasks.empty()) {
                notification.wait(lock);
                continue;
            } else {
                task = pendingTasks.front();
                pendingTasks.pop_front();
            }
        }

        task();
    }
}

void Executor::submit(const Executor::Task& task) {
    std::unique_lock<std::mutex> lock(mutex);

    pendingTasks.push_back(task);
    notification.notify_one();
}
