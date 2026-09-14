#include <chrono>

#include <gtest/gtest.h>

#include "inferx/metrics.hpp"

TEST(MetricsTest, RecordsRequestsAndBatches)
{
    inferx::Metrics metrics;

    const auto start =
        inferx::Metrics::Clock::now();

    const auto end =
        start +
        std::chrono::milliseconds(50);

    metrics.recordRequest(
        start,
        end
    );

    metrics.recordBatch(1);

    EXPECT_EQ(
        metrics.requestCount(),
        1
    );

    EXPECT_EQ(
        metrics.batchCount(),
        1
    );

    EXPECT_DOUBLE_EQ(
        metrics.averageLatencyMs(),
        50.0
    );

    EXPECT_DOUBLE_EQ(
        metrics.averageBatchSize(),
        1.0
    );
}

TEST(MetricsTest, CalculatesPercentiles)
{
    inferx::Metrics metrics;

    const auto base =
        inferx::Metrics::Clock::now();

    metrics.recordRequest(
        base,
        base + std::chrono::milliseconds(10)
    );

    metrics.recordRequest(
        base,
        base + std::chrono::milliseconds(20)
    );

    metrics.recordRequest(
        base,
        base + std::chrono::milliseconds(30)
    );

    metrics.recordRequest(
        base,
        base + std::chrono::milliseconds(40)
    );

    EXPECT_DOUBLE_EQ(
        metrics.percentileLatencyMs(50.0),
        20.0
    );

    EXPECT_DOUBLE_EQ(
        metrics.percentileLatencyMs(100.0),
        40.0
    );
}