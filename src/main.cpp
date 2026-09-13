#include <chrono>
#include <iomanip>
#include <iostream>
#include <utility>

#include "inferx/batch.hpp"
#include "inferx/dynamic_batcher.hpp"
#include "inferx/inference_request.hpp"
#include "inferx/metrics.hpp"
#include "inferx/worker_pool.hpp"

int main()
{
    std::cout
        << "=====================================\n";

    std::cout
        << "             InferX v0.7             \n";

    std::cout
        << " AI Inference Scheduler Simulator    \n";

    std::cout
        << "=====================================\n\n";

    constexpr std::size_t maxBatchSize = 3;
    constexpr std::size_t workerCount = 2;

    inferx::Metrics metrics;

    inferx::DynamicBatcher batcher(
        maxBatchSize,
        std::chrono::milliseconds(100)
    );

    inferx::WorkerPool workers(
        workerCount,
        metrics
    );

    batcher.addRequest(
        inferx::InferenceRequest(
            1,
            "text-model",
            inferx::Priority::Normal,
            50
        )
    );

    batcher.addRequest(
        inferx::InferenceRequest(
            2,
            "vision-model",
            inferx::Priority::High,
            30
        )
    );

    batcher.addRequest(
        inferx::InferenceRequest(
            3,
            "embedding-model",
            inferx::Priority::Low,
            20
        )
    );

    batcher.addRequest(
        inferx::InferenceRequest(
            4,
            "speech-model",
            inferx::Priority::High,
            40
        )
    );

    batcher.addRequest(
        inferx::InferenceRequest(
            5,
            "ranking-model",
            inferx::Priority::Normal,
            25
        )
    );

    batcher.addRequest(
        inferx::InferenceRequest(
            6,
            "recommendation-model",
            inferx::Priority::Normal,
            35
        )
    );

    std::cout
        << "Requests received: "
        << batcher.waitingCount()
        << '\n';

    std::cout
        << "Worker threads: "
        << workerCount
        << '\n';

    std::cout
        << "Maximum batch size: "
        << maxBatchSize
        << "\n\n";

    int batchNumber = 1;

    while (!batcher.empty())
    {
        inferx::Batch batch =
            batcher.createBatch();

        std::cout
            << "Submitting Batch "
            << batchNumber
            << " with "
            << batch.size()
            << " requests.\n";

        workers.submit(
            std::move(batch)
        );

        ++batchNumber;
    }

    std::cout
        << "\nWaiting for workers to finish...\n\n";

    workers.shutdown();

    std::cout
        << "All inference batches completed.\n\n";

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "=====================================\n";

    std::cout
        << "      InferX Performance Report      \n";

    std::cout
        << "=====================================\n";

    std::cout
        << "Requests processed : "
        << metrics.requestCount()
        << '\n';

    std::cout
        << "Batches processed  : "
        << metrics.batchCount()
        << '\n';

    std::cout
        << "Average latency    : "
        << metrics.averageLatencyMs()
        << " ms\n";

    std::cout
        << "P50 latency        : "
        << metrics.percentileLatencyMs(50.0)
        << " ms\n";

    std::cout
        << "P95 latency        : "
        << metrics.percentileLatencyMs(95.0)
        << " ms\n";

    std::cout
        << "P99 latency        : "
        << metrics.percentileLatencyMs(99.0)
        << " ms\n";

    std::cout
        << "Throughput         : "
        << metrics.throughputRequestsPerSecond()
        << " requests/sec\n";

    std::cout
        << "Average batch size : "
        << metrics.averageBatchSize()
        << '\n';

    std::cout
        << "=====================================\n";

    return 0;
}