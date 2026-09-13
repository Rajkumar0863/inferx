#include "inferx/dynamic_batcher.hpp"

#include <algorithm>
#include <stdexcept>

namespace inferx
{
    DynamicBatcher::DynamicBatcher(std::size_t maxBatchSize)
        : maxBatchSize_(maxBatchSize)
    {
        if (maxBatchSize_ == 0)
        {
            throw std::invalid_argument(
                "Maximum batch size must be greater than zero."
            );
        }
    }

    void DynamicBatcher::addRequest(
        const InferenceRequest& request
    )
    {
        waitingQueue_.push(request);
    }

    bool DynamicBatcher::hasReadyBatch() const
    {
        return waitingQueue_.size() >= maxBatchSize_;
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