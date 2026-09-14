#include <gtest/gtest.h>

#include "inferx/fifo_scheduler.hpp"
#include "inferx/inference_request.hpp"

TEST(FifoSchedulerTest, PreservesInsertionOrder)
{
    inferx::FifoScheduler scheduler;

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
            20
        )
    );

    scheduler.enqueue(
        inferx::InferenceRequest(
            3,
            "embedding-model",
            inferx::Priority::Low,
            30
        )
    );

    EXPECT_EQ(scheduler.size(), 3);

    EXPECT_EQ(
        scheduler.dequeue().getRequestId(),
        1
    );

    EXPECT_EQ(
        scheduler.dequeue().getRequestId(),
        2
    );

    EXPECT_EQ(
        scheduler.dequeue().getRequestId(),
        3
    );

    EXPECT_TRUE(scheduler.empty());
}

TEST(FifoSchedulerTest, ThrowsWhenDequeuingEmptyQueue)
{
    inferx::FifoScheduler scheduler;

    EXPECT_THROW(
        scheduler.dequeue(),
        std::runtime_error
    );
}