#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

namespace inferx
{
    template <typename T>
    class ThreadSafeQueue
    {
    public:
        ThreadSafeQueue() = default;

        ThreadSafeQueue(const ThreadSafeQueue&) = delete;
        ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

        void push(T item)
        {
            {
                std::lock_guard<std::mutex> lock(mutex_);

                if (stopped_)
                {
                    return;
                }

                queue_.push(std::move(item));
            }

            condition_.notify_one();
        }

        bool waitAndPop(T& item)
        {
            std::unique_lock<std::mutex> lock(mutex_);

            condition_.wait(
                lock,
                [this]
                {
                    return stopped_ || !queue_.empty();
                }
            );

            if (queue_.empty())
            {
                return false;
            }

            item = std::move(queue_.front());
            queue_.pop();

            return true;
        }

        void shutdown()
        {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                stopped_ = true;
            }

            condition_.notify_all();
        }

        bool empty() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.empty();
        }

    private:
        mutable std::mutex mutex_;

        std::condition_variable condition_;

        std::queue<T> queue_;

        bool stopped_ = false;
    };
}