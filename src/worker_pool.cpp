#include "inferx/worker_pool.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

namespace inferx
{
    WorkerPool::WorkerPool(
        std::size_t workerCount,
        Metrics& metrics
    )
        : metrics_(metrics)
    {
        if (workerCount == 0)
        {
            throw std::invalid_argument(
                "Worker count must be greater than zero."
            );
        }

        workers_.reserve(workerCount);

        for (std::size_t i = 0; i < workerCount; ++i)
        {
            workers_.emplace_back(
                &WorkerPool::workerLoop,
                this,
                i + 1
            );
        }
    }

    WorkerPool::~WorkerPool()
    {
        shutdown();
    }

    void WorkerPool::submit(Batch batch)
    {
        if (stopped_)
        {
            throw std::runtime_error(
                "Cannot submit a batch after worker pool shutdown."
            );
        }

        batchQueue_.push(
            std::move(batch)
        );
    }

    void WorkerPool::shutdown()
    {
        bool expected = false;

        if (!stopped_.compare_exchange_strong(
                expected,
                true
            ))
        {
            return;
        }

        batchQueue_.shutdown();

        for (auto& worker : workers_)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }

    void WorkerPool::workerLoop(
        std::size_t workerId
    )
    {
        Batch batch;

        while (batchQueue_.waitAndPop(batch))
        {
            processBatch(
                workerId,
                batch
            );
        }
    }

    void WorkerPool::processBatch(
        std::size_t workerId,
        const Batch& batch
    )
    {
        int simulatedProcessingTimeMs = 0;

        for (const auto& request :
             batch.getRequests())
        {
            simulatedProcessingTimeMs =
                std::max(
                    simulatedProcessingTimeMs,
                    request.getProcessingTimeMs()
                );
        }

        {
            std::lock_guard<std::mutex>
                lock(outputMutex_);

            std::cout
                << "Worker "
                << workerId
                << " started batch of "
                << batch.size()
                << " request(s).\n";

            std::cout << "  Requests: ";

            for (const auto& request :
                 batch.getRequests())
            {
                std::cout
                    << request.getRequestId()
                    << ' ';
            }

            std::cout << '\n';
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                simulatedProcessingTimeMs
            )
        );

        const auto completionTime =
            Metrics::Clock::now();

        for (const auto& request :
             batch.getRequests())
        {
            metrics_.recordRequest(
                request.getArrivalTime(),
                completionTime
            );
        }

        metrics_.recordBatch(
            batch.size()
        );

        {
            std::lock_guard<std::mutex>
                lock(outputMutex_);

            std::cout
                << "Worker "
                << workerId
                << " completed batch in approximately "
                << simulatedProcessingTimeMs
                << " ms.\n\n";
        }
    }
}