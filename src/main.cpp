#include <chrono>
#include <iostream>

#include "inferx/batch.hpp"
#include "inferx/dynamic_batcher.hpp"
#include "inferx/inference_request.hpp"
#include "inferx/worker_pool.hpp"

int main()
{
    std::cout << "=====================================\n";
    std::cout << "             InferX v0.6             \n";
    std::cout << " AI Inference Scheduler Simulator    \n";
    std::cout << "=====================================\n\n";

    constexpr std::size_t maxBatchSize = 3;
    constexpr std::size_t workerCount = 2;

    inferx::DynamicBatcher batcher(
        maxBatchSize,
        std::chrono::milliseconds(100)
    );

    inferx::WorkerPool workers(workerCount);

    inferx::InferenceRequest request1(
        1,
        "text-model",
        inferx::Priority::Normal,
        50
    );

    inferx::InferenceRequest request2(
        2,
        "vision-model",
        inferx::Priority::High,
        30
    );

    inferx::InferenceRequest request3(
        3,
        "embedding-model",
        inferx::Priority::Low,
        20
    );

    inferx::InferenceRequest request4(
        4,
        "speech-model",
        inferx::Priority::High,
        40
    );

    inferx::InferenceRequest request5(
        5,
        "ranking-model",
        inferx::Priority::Normal,
        25
    );

    inferx::InferenceRequest request6(
        6,
        "recommendation-model",
        inferx::Priority::Normal,
        35
    );

    batcher.addRequest(request1);
    batcher.addRequest(request2);
    batcher.addRequest(request3);
    batcher.addRequest(request4);
    batcher.addRequest(request5);
    batcher.addRequest(request6);

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
        inferx::Batch batch = batcher.createBatch();

        std::cout
            << "Submitting Batch "
            << batchNumber
            << " with "
            << batch.size()
            << " requests.\n";

        workers.submit(std::move(batch));

        ++batchNumber;
    }

    std::cout
        << "\nWaiting for workers to finish...\n\n";

    workers.shutdown();

    std::cout
        << "All inference batches completed.\n";

    return 0;
}