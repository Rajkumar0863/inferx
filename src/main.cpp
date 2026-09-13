#include <iostream>

#include "inferx/inference_request.hpp"

int main()
{
    std::cout << "=====================================\n";
    std::cout << "             InferX v0.1             \n";
    std::cout << " AI Inference Scheduler Simulator    \n";
    std::cout << "=====================================\n\n";

    inferx::InferenceRequest request(
        1,
        "text-model",
        inferx::Priority::High,
        50
    );

    std::cout << "Request created successfully.\n";
    std::cout << "Request ID: " << request.getRequestId() << '\n';
    std::cout << "Model: " << request.getModelName() << '\n';
    std::cout << "Processing Time: "
              << request.getProcessingTimeMs()
              << " ms\n";

    return 0;
}