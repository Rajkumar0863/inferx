#include <iostream>

#include "inferx/fifo_scheduler.hpp"
#include "inferx/inference_request.hpp"

int main()
{
    std::cout << "=====================================\n";
    std::cout << "             InferX v0.2             \n";
    std::cout << " AI Inference Scheduler Simulator    \n";
    std::cout << "=====================================\n\n";

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

    inferx::FifoScheduler scheduler;

    scheduler.enqueue(request1);
    scheduler.enqueue(request2);
    scheduler.enqueue(request3);

    std::cout << "Requests added to FIFO scheduler.\n";
    std::cout << "Queue size: " << scheduler.size() << "\n\n";

    std::cout << "Dequeue order:\n";

    while (!scheduler.empty())
    {
        inferx::InferenceRequest request = scheduler.dequeue();

        std::cout
            << "Request ID: "
            << request.getRequestId()
            << " | Model: "
            << request.getModelName()
            << " | Processing Time: "
            << request.getProcessingTimeMs()
            << " ms\n";
    }

    return 0;
}