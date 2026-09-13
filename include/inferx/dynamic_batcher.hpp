#pragma once

#include <chrono>
#include <cstddef>
#include <queue>

#include "inferx/batch.hpp"
#include "inferx/inference_request.hpp"

namespace inferx
{
    class DynamicBatcher
    {
    public:
        using Clock = std::chrono::steady_clock;

        DynamicBatcher(
            std::size_t maxBatchSize,
            std::chrono::milliseconds maxWaitTime
        );

        void addRequest(const InferenceRequest& request);

        bool hasReadyBatch() const;

        Batch createBatch();

        bool empty() const;

        std::size_t waitingCount() const;

    private:
        bool hasTimedOut() const;

        std::size_t maxBatchSize_;

        std::chrono::milliseconds maxWaitTime_;

        std::queue<InferenceRequest> waitingQueue_;
    };
}