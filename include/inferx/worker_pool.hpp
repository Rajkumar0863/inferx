#pragma once

#include <atomic>
#include <cstddef>
#include <mutex>
#include <thread>
#include <vector>

#include "inferx/batch.hpp"
#include "inferx/metrics.hpp"
#include "inferx/thread_safe_queue.hpp"

namespace inferx
{
    class WorkerPool
    {
    public:
        WorkerPool(
            std::size_t workerCount,
            Metrics& metrics
        );

        ~WorkerPool();

        WorkerPool(const WorkerPool&) = delete;
        WorkerPool& operator=(const WorkerPool&) = delete;

        void submit(Batch batch);

        void shutdown();

    private:
        void workerLoop(std::size_t workerId);

        void processBatch(
            std::size_t workerId,
            const Batch& batch
        );

        ThreadSafeQueue<Batch> batchQueue_;

        std::vector<std::thread> workers_;

        std::atomic<bool> stopped_{false};

        std::mutex outputMutex_;

        Metrics& metrics_;
    };
}