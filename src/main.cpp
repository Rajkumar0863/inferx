#include <chrono>
#include <iostream>
#include <thread>

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
            << '\n';
    }

    std::cout << '\n';
}

int main()
{
    std::cout << "=====================================\n";
    std::cout << "             InferX v0.5             \n";
    std::cout << " AI Inference Scheduler Simulator    \n";
    std::cout << "=====================================\n\n";

    inferx::DynamicBatcher batcher(
        4,
        std::chrono::milliseconds(100)
    );

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

    batcher.addRequest(request1);
    batcher.addRequest(request2);

    std::cout
        << "Requests waiting: "
        << batcher.waitingCount()
        << '\n';

    std::cout
        << "Ready immediately? "
        << (batcher.hasReadyBatch() ? "YES" : "NO")
        << "\n\n";

    std::cout
        << "Waiting 150 ms to trigger timeout...\n\n";

    std::this_thread::sleep_for(
        std::chrono::milliseconds(150)
    );

    std::cout
        << "Ready after timeout? "
        << (batcher.hasReadyBatch() ? "YES" : "NO")
        << "\n\n";

    if (batcher.hasReadyBatch())
    {
        inferx::Batch batch = batcher.createBatch();

        printBatch(batch, 1);
    }

    return 0;
}