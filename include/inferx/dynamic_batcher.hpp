#pragma once

#include <cstddef>
#include <queue>

#include "inferx/batch.hpp"
#include "inferx/inference_request.hpp"

namespace inferx
{
    class DynamicBatcher
    {
    public:
        explicit DynamicBatcher(std::size_t maxBatchSize);

        void addRequest(const InferenceRequest& request);

        bool hasReadyBatch() const;

        Batch createBatch();

        bool empty() const;

        std::size_t waitingCount() const;

    private:
        std::size_t maxBatchSize_;

        std::queue<InferenceRequest> waitingQueue_;
    };
}