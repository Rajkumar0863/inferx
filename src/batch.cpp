#include "inferx/batch.hpp"

namespace inferx
{
    Batch::Batch() = default;

    void Batch::addRequest(const InferenceRequest& request)
    {
        requests_.push_back(request);
    }

    bool Batch::empty() const
    {
        return requests_.empty();
    }

    std::size_t Batch::size() const
    {
        return requests_.size();
    }

    const std::vector<InferenceRequest>& Batch::getRequests() const
    {
        return requests_;
    }
}