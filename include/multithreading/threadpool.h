#pragma once

#include <cassert>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <stop_token>
#include <thread>
#include <vector>

namespace Multithreading
{
class ThreadPool
{
public:
    explicit ThreadPool(unsigned threadCount);
    ~ThreadPool();

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;

    template <typename Func, typename... Args>
    auto addTask(Func &&func, Args &&...args)
    {
        using ReturnType = std::invoke_result_t<Func, Args...>;

        auto task{ std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...)) };
        auto future{ task->get_future() };

        std::lock_guard lock(_queueMutex);
        _queue.emplace([task = std::move(task)]() { (*task)(); });

        _condition.notify_one();
        return future;
    }

private:
    void worker(std::stop_token stopToken);

    std::vector<std::jthread> _threads;
    std::queue<std::function<void()>> _queue;
    std::mutex _queueMutex;
    std::condition_variable _condition;
};
} // namespace Multithreading
