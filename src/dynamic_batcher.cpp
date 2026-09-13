#include "inferx/dynamic_batcher.hpp"

#include <algorithm>
#include <stdexcept>

namespace inferx
{
    DynamicBatcher::DynamicBatcher(
        std::size_t maxBatchSize,
        std::chrono::milliseconds maxWaitTime
    )
        : maxBatchSize_(maxBatchSize),
          maxWaitTime_(maxWaitTime)
    {
        if (maxBatchSize_ == 0)
        {
            throw std::invalid_argument(
                "Maximum batch size must be greater than zero."
            );
        }

        if (maxWaitTime_.count() < 0)
        {
            throw std::invalid_argument(
                "Maximum wait time cannot be negative."
            );
        }
    }

    void DynamicBatcher::addRequest(
        const InferenceRequest& request
    )
    {
        waitingQueue_.push(request);
    }

    bool DynamicBatcher::hasTimedOut() const
    {
        if (waitingQueue_.empty())
        {
            return false;
        }

        const auto now = Clock::now();

        const auto oldestArrival =
            waitingQueue_.front().getArrivalTime();

        const auto waitingTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - oldestArrival
            );

        return waitingTime >= maxWaitTime_;
    }

    bool DynamicBatcher::hasReadyBatch() const
    {
        if (waitingQueue_.empty())
        {
            return false;
        }

        return waitingQueue_.size() >= maxBatchSize_
            || hasTimedOut();
    }

    Batch DynamicBatcher::createBatch()
    {
        if (waitingQueue_.empty())
        {
            throw std::runtime_error(
                "Cannot create a batch from an empty queue."
            );
        }

        Batch batch;

        const std::size_t batchSize =
            std::min(maxBatchSize_, waitingQueue_.size());

        for (std::size_t i = 0; i < batchSize; ++i)
        {
            batch.addRequest(waitingQueue_.front());
            waitingQueue_.pop();
        }

        return batch;
    }

    bool DynamicBatcher::empty() const
    {
        return waitingQueue_.empty();
    }

    std::size_t DynamicBatcher::waitingCount() const
    {
        return waitingQueue_.size();
    }
}