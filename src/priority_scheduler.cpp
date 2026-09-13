#include "inferx/priority_scheduler.hpp"

#include <stdexcept>

namespace inferx
{
    bool PriorityComparator::operator()(
        const InferenceRequest& lhs,
        const InferenceRequest& rhs
    ) const
    {
        if (lhs.getPriority() == rhs.getPriority())
        {
            return lhs.getArrivalTime() > rhs.getArrivalTime();
        }

        return static_cast<int>(lhs.getPriority()) <
               static_cast<int>(rhs.getPriority());
    }

    void PriorityScheduler::enqueue(const InferenceRequest& request)
    {
        queue_.push(request);
    }

    bool PriorityScheduler::empty() const
    {
        return queue_.empty();
    }

    std::size_t PriorityScheduler::size() const
    {
        return queue_.size();
    }

    InferenceRequest PriorityScheduler::dequeue()
    {
        if (queue_.empty())
        {
            throw std::runtime_error(
                "Cannot dequeue from an empty priority scheduler."
            );
        }

        InferenceRequest request = queue_.top();
        queue_.pop();

        return request;
    }
}