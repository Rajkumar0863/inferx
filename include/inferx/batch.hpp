#pragma once

#include <cstddef>
#include <vector>

#include "inferx/inference_request.hpp"

namespace inferx
{
    class Batch
    {
    public:
        Batch();

        void addRequest(const InferenceRequest& request);

        bool empty() const;

        std::size_t size() const;

        const std::vector<InferenceRequest>& getRequests() const;

    private:
        std::vector<InferenceRequest> requests_;
    };
}