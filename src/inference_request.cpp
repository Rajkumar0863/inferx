#include "inferx/inference_request.hpp"

#include <utility>

namespace inferx
{
    InferenceRequest::InferenceRequest(
        int requestId,
        std::string modelName,
        Priority priority,
        int processingTimeMs
    )
        : requestId_(requestId),
          modelName_(std::move(modelName)),
          priority_(priority),
          processingTimeMs_(processingTimeMs),
          arrivalTime_(Clock::now())
    {
    }

    int InferenceRequest::getRequestId() const
    {
        return requestId_;
    }

    const std::string& InferenceRequest::getModelName() const
    {
        return modelName_;
    }

    Priority InferenceRequest::getPriority() const
    {
        return priority_;
    }

    int InferenceRequest::getProcessingTimeMs() const
    {
        return processingTimeMs_;
    }

    InferenceRequest::TimePoint InferenceRequest::getArrivalTime() const
    {
        return arrivalTime_;
    }
}