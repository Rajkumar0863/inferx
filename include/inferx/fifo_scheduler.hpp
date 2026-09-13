#pragma once

#include <cstddef>
#include <queue>

#include "inferx/inference_request.hpp"

namespace inferx
{
    class FifoScheduler
    {
    public:
        void enqueue(const InferenceRequest& request);

        bool empty() const;

        std::size_t size() const;

        InferenceRequest dequeue();

    private:
        std::queue<InferenceRequest> queue_;
    };
}