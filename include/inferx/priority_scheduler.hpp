#pragma once

#include <cstddef>
#include <queue>
#include <vector>

#include "inferx/inference_request.hpp"

namespace inferx
{
    struct PriorityComparator
    {
        bool operator()(
            const InferenceRequest& lhs,
            const InferenceRequest& rhs
        ) const;
    };

    class PriorityScheduler
    {
    public:
        void enqueue(const InferenceRequest& request);

        bool empty() const;

        std::size_t size() const;

        InferenceRequest dequeue();

    private:
        std::priority_queue<
            InferenceRequest,
            std::vector<InferenceRequest>,
            PriorityComparator
        > queue_;
    };
}