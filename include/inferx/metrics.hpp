#pragma once

#include <chrono>
#include <cstddef>
#include <mutex>
#include <vector>

namespace inferx
{
    class Metrics
    {
    public:
        using Clock = std::chrono::steady_clock;
        using TimePoint = Clock::time_point;

        Metrics();

        void recordRequest(
            TimePoint arrivalTime,
            TimePoint completionTime
        );

        void recordBatch(std::size_t batchSize);

        std::size_t requestCount() const;

        std::size_t batchCount() const;

        double averageLatencyMs() const;

        double percentileLatencyMs(double percentile) const;

        double throughputRequestsPerSecond() const;

        double averageBatchSize() const;

    private:
        std::vector<double> snapshotLatencies() const;

        mutable std::mutex mutex_;

        std::vector<double> latenciesMs_;

        std::size_t totalRequests_{0};

        std::size_t totalBatches_{0};

        std::size_t totalRequestsInBatches_{0};

        TimePoint startTime_;
    };
}