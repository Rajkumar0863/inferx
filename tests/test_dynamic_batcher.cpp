#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include "inferx/dynamic_batcher.hpp"
#include "inferx/inference_request.hpp"

TEST(DynamicBatcherTest, CreatesBatchAtMaximumSize)
{
    inferx::DynamicBatcher batcher(
        3,
        std::chrono::milliseconds(100)
    );

    batcher.addRequest(
        inferx::InferenceRequest(
            1,
            "model-a",
            inferx::Priority::Normal,
            10
        )
    );

    batcher.addRequest(
        inferx::InferenceRequest(
            2,
            "model-b",
            inferx::Priority::Normal,
            10
        )
    );

    batcher.addRequest(
        inferx::InferenceRequest(
            3,
            "model-c",
            inferx::Priority::Normal,
            10
        )
    );

    EXPECT_TRUE(
        batcher.hasReadyBatch()
    );

    inferx::Batch batch =
        batcher.createBatch();

    EXPECT_EQ(
        batch.size(),
        3
    );

    EXPECT_TRUE(
        batcher.empty()
    );
}

TEST(DynamicBatcherTest, BecomesReadyAfterTimeout)
{
    inferx::DynamicBatcher batcher(
        4,
        std::chrono::milliseconds(20)
    );

    batcher.addRequest(
        inferx::InferenceRequest(
            1,
            "model-a",
            inferx::Priority::Normal,
            10
        )
    );

    EXPECT_FALSE(
        batcher.hasReadyBatch()
    );

    std::this_thread::sleep_for(
        std::chrono::milliseconds(30)
    );

    EXPECT_TRUE(
        batcher.hasReadyBatch()
    );
}