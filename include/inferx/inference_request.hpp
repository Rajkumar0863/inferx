#pragma once

#include <chrono>
#include <string>

namespace inferx
{
    enum class Priority
    {
        Low = 1,
        Normal = 2,
        High = 3
    };

    class InferenceRequest
    {
    public:
        using Clock = std::chrono::steady_clock;
        using TimePoint = Clock::time_point;

        InferenceRequest(
            int requestId,
            std::string modelName,
            Priority priority,
            int processingTimeMs
        );

        int getRequestId() const;
        const std::string& getModelName() const;
        Priority getPriority() const;
        int getProcessingTimeMs() const;
        TimePoint getArrivalTime() const;

    private:
        int requestId_;
        std::string modelName_;
        Priority priority_;
        int processingTimeMs_;
        TimePoint arrivalTime_;
    };
}