#include "threadpool.h"

#include <iostream>

namespace Multithreading
{

ThreadPool::ThreadPool(unsigned threadCount)
{
    _threads.reserve(threadCount);
    for (unsigned i = 0; i < threadCount; ++i)
        _threads.emplace_back([this](std::stop_token st) { worker(st); });
}

ThreadPool::~ThreadPool()
{
    for (auto &thread : _threads)
        thread.request_stop();

    _condition.notify_all();
}

void ThreadPool::worker(std::stop_token stopToken)
{
    while (!stopToken.stop_requested())
    {
        std::unique_lock lock(_queueMutex);
        _condition.wait(lock, [this, &stopToken]()
                        { return stopToken.stop_requested() || !_queue.empty(); });

        if (stopToken.stop_requested())
            return;

        const std::function<void()> task{ std::move(_queue.front()) };
        _queue.pop();
        lock.unlock();

        try
        {
            task();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Worker caught exception: " << e.what() << '\n';
        }
        catch (...)
        {
            std::cerr << "Worker caught unknown exception\n";
        }
    }
}

} // namespace Multithreading
