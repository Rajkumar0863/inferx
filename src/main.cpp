#include <iostream>

#include "inferx/batch.hpp"
#include "inferx/dynamic_batcher.hpp"
#include "inferx/inference_request.hpp"

void printBatch(
    const inferx::Batch& batch,
    int batchNumber
)
{
    std::cout
        << "Batch "
        << batchNumber
        << " | Size: "
        << batch.size()
        << '\n';

    for (const auto& request : batch.getRequests())
    {
        std::cout
            << "  Request ID: "
            << request.getRequestId()
            << " | Model: "
            << request.getModelName()
            << " | Processing Time: "
            << request.getProcessingTimeMs()
            << " ms\n";
    }

    std::cout << '\n';
}

int main()
{
    std::cout << "=====================================\n";
    std::cout << "             InferX v0.4             \n";
    std::cout << " AI Inference Scheduler Simulator    \n";
    std::cout << "=====================================\n\n";

    inferx::DynamicBatcher batcher(4);

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
        << "Requests waiting: "
        << batcher.waitingCount()
        << "\n\n";

    int batchNumber = 1;

    while (!batcher.empty())
    {
        inferx::Batch batch = batcher.createBatch();

        printBatch(batch, batchNumber);

        ++batchNumber;
    }

    return 0;
}