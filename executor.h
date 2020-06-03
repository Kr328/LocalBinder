#pragma once

#include <thread>
#include <list>
#include <stack>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>

#define DEFAULT_THREAD_POOL_SIZE 8

class Executor {
public:
    typedef std::function<void()> Task;

public:
    explicit Executor(int threadCount);
    ~Executor();

public:
    static Executor *getInstance();

public:
    void submit(const Task &task);

private:
    void runThread();

private:
    bool closed;
    std::mutex mutex;
    std::condition_variable notification;
    std::list<Task> pendingTasks;
    std::vector<std::thread> threads;

private:
    static Executor *instance;
};
