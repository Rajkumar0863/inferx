#include <gtest/gtest.h>

#include "inferx/inference_request.hpp"
#include "inferx/priority_scheduler.hpp"

TEST(PrioritySchedulerTest, ProcessesHigherPriorityFirst)
{
    inferx::PriorityScheduler scheduler;

    scheduler.enqueue(
        inferx::InferenceRequest(
            1,
            "text-model",
            inferx::Priority::Normal,
            10
        )
    );

    scheduler.enqueue(
        inferx::InferenceRequest(
            2,
            "vision-model",
            inferx::Priority::High,
            10
        )
    );

    scheduler.enqueue(
        inferx::InferenceRequest(
            3,
            "embedding-model",
            inferx::Priority::Low,
            10
        )
    );

    EXPECT_EQ(
        scheduler.dequeue().getRequestId(),
        2
    );

    EXPECT_EQ(
        scheduler.dequeue().getRequestId(),
        1
    );

    EXPECT_EQ(
        scheduler.dequeue().getRequestId(),
        3
    );
}

TEST(PrioritySchedulerTest, PreservesOrderForEqualPriority)
{
    inferx::PriorityScheduler scheduler;

    scheduler.enqueue(
        inferx::InferenceRequest(
            1,
            "model-a",
            inferx::Priority::High,
            10
        )
    );

    scheduler.enqueue(
        inferx::InferenceRequest(
            2,
            "model-b",
            inferx::Priority::High,
            10
        )
    );

    EXPECT_EQ(
        scheduler.dequeue().getRequestId(),
        1
    );

    EXPECT_EQ(
        scheduler.dequeue().getRequestId(),
        2
    );
}