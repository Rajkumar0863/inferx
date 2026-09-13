#include "inferx/metrics.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace inferx
{
    Metrics::Metrics()
        : startTime_(Clock::now())
    {
    }

    void Metrics::recordRequest(
        TimePoint arrivalTime,
        TimePoint completionTime
    )
    {
        const auto latency =
            std::chrono::duration<double, std::milli>(
                completionTime - arrivalTime
            ).count();

        std::lock_guard<std::mutex> lock(mutex_);

        latenciesMs_.push_back(latency);
        ++totalRequests_;
    }

    void Metrics::recordBatch(std::size_t batchSize)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        ++totalBatches_;
        totalRequestsInBatches_ += batchSize;
    }

    std::size_t Metrics::requestCount() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return totalRequests_;
    }

    std::size_t Metrics::batchCount() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return totalBatches_;
    }

    std::vector<double> Metrics::snapshotLatencies() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return latenciesMs_;
    }

    double Metrics::averageLatencyMs() const
    {
        const auto latencies = snapshotLatencies();

        if (latencies.empty())
        {
            return 0.0;
        }

        const double total =
            std::accumulate(
                latencies.begin(),
                latencies.end(),
                0.0
            );

        return total /
               static_cast<double>(latencies.size());
    }

    double Metrics::percentileLatencyMs(
        double percentile
    ) const
    {
        if (percentile < 0.0 || percentile > 100.0)
        {
            throw std::invalid_argument(
                "Percentile must be between 0 and 100."
            );
        }

        auto latencies = snapshotLatencies();

        if (latencies.empty())
        {
            return 0.0;
        }

        std::sort(
            latencies.begin(),
            latencies.end()
        );

        if (percentile == 0.0)
        {
            return latencies.front();
        }

        const double rank =
            (percentile / 100.0) *
            static_cast<double>(latencies.size());

        std::size_t index =
            static_cast<std::size_t>(
                std::ceil(rank)
            );

        if (index == 0)
        {
            index = 1;
        }

        index -= 1;

        if (index >= latencies.size())
        {
            index = latencies.size() - 1;
        }

        return latencies[index];
    }

    double Metrics::throughputRequestsPerSecond() const
    {
        std::size_t requests = 0;

        {
            std::lock_guard<std::mutex> lock(mutex_);
            requests = totalRequests_;
        }

        if (requests == 0)
        {
            return 0.0;
        }

        const auto now = Clock::now();

        const double elapsedSeconds =
            std::chrono::duration<double>(
                now - startTime_
            ).count();

        if (elapsedSeconds <= 0.0)
        {
            return 0.0;
        }

        return static_cast<double>(requests) /
               elapsedSeconds;
    }

    double Metrics::averageBatchSize() const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (totalBatches_ == 0)
        {
            return 0.0;
        }

        return
            static_cast<double>(
                totalRequestsInBatches_
            ) /
            static_cast<double>(
                totalBatches_
            );
    }
}