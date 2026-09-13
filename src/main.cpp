#include <iostream>
#include <string>

#include "inferx/inference_request.hpp"
#include "inferx/priority_scheduler.hpp"

std::string priorityToString(inferx::Priority priority)
{
    switch (priority)
    {
        case inferx::Priority::High:
            return "HIGH";

        case inferx::Priority::Normal:
            return "NORMAL";

        case inferx::Priority::Low:
            return "LOW";
    }

    return "UNKNOWN";
}

int main()
{
    std::cout << "=====================================\n";
    std::cout << "             InferX v0.3             \n";
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

    inferx::PriorityScheduler scheduler;

    scheduler.enqueue(request1);
    scheduler.enqueue(request2);
    scheduler.enqueue(request3);
    scheduler.enqueue(request4);
    scheduler.enqueue(request5);

    std::cout << "Requests added to priority scheduler.\n";
    std::cout << "Queue size: " << scheduler.size() << "\n\n";

    std::cout << "Dequeue order:\n";

    while (!scheduler.empty())
    {
        inferx::InferenceRequest request = scheduler.dequeue();

        std::cout
            << "Request ID: "
            << request.getRequestId()
            << " | Priority: "
            << priorityToString(request.getPriority())
            << " | Model: "
            << request.getModelName()
            << " | Processing Time: "
            << request.getProcessingTimeMs()
            << " ms\n";
    }

    return 0;
}