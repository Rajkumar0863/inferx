#include "inferx/fifo_scheduler.hpp"

#include <stdexcept>

namespace inferx
{
    void FifoScheduler::enqueue(const InferenceRequest& request)
    {
        queue_.push(request);
    }

    bool FifoScheduler::empty() const
    {
        return queue_.empty();
    }

    std::size_t FifoScheduler::size() const
    {
        return queue_.size();
    }

    InferenceRequest FifoScheduler::dequeue()
    {
        if (queue_.empty())
        {
            throw std::runtime_error(
                "Cannot dequeue from an empty FIFO scheduler."
            );
        }

        InferenceRequest request = queue_.front();
        queue_.pop();

        return request;
    }
}