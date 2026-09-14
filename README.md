# InferX

**InferX** is a C++20 AI inference scheduling simulator built to explore the systems concepts behind serving machine-learning inference workloads.

The project currently models incoming inference requests, scheduling policies, dynamic batching, concurrent batch execution through a worker pool, and runtime performance measurement.

InferX is being developed incrementally, with each milestone introducing another component of an inference-serving pipeline.

> **Current version:** v0.8  
> **Status:** Active development

---

## Why InferX?

Modern AI inference systems often receive many requests concurrently.

Executing every request independently can lead to inefficient resource usage, while poor scheduling can increase latency for important requests.

Inference-serving systems therefore need mechanisms such as:

- request queues
- scheduling policies
- priority handling
- dynamic batching
- concurrent workers
- latency measurement
- throughput monitoring

InferX provides a simplified environment for implementing and experimenting with these concepts in modern C++.

The project does **not** execute real neural-network models or GPU kernels. Processing time is currently simulated so that the scheduling and concurrency behaviour can be studied independently.

---

## Current Architecture

```text
                  +----------------------+
                  |   Inference Request  |
                  +----------+-----------+
                             |
                             v
                  +----------------------+
                  |      Scheduler       |
                  |  FIFO / Priority     |
                  +----------+-----------+
                             |
                             v
                  +----------------------+
                  |   Dynamic Batcher    |
                  +----------+-----------+
                             |
                             v
                  +----------------------+
                  |  Thread-Safe Queue   |
                  +----------+-----------+
                             |
                    +--------+--------+
                    |                 |
                    v                 v
              +-----------+     +-----------+
              | Worker 1  |     | Worker 2  |
              +-----+-----+     +-----+-----+
                    |                 |
                    +--------+--------+
                             |
                             v
                  +----------------------+
                  |    Metrics Engine    |
                  +----------------------+
                    |   |   |   |   |
                    v   v   v   v   v
                   Avg P50 P95 P99 Throughput
```

---

## Implemented Features

### Inference Request Model

InferX represents an inference workload using an `InferenceRequest`.

A request contains information used by the scheduling simulator, including:

- request identifier
- model name
- priority
- simulated processing time
- arrival time

The arrival timestamp is used later by the metrics engine to calculate end-to-end request latency.

---

### FIFO Scheduler

The FIFO scheduler processes requests in the same order in which they arrive.

```text
Request 1
Request 2
Request 3

     ↓

1 → 2 → 3
```

This provides the baseline scheduling strategy.

---

### Priority Scheduler

InferX also supports priority-based scheduling.

Requests can currently use priority levels such as:

```text
HIGH
NORMAL
LOW
```

Higher-priority requests are selected before lower-priority requests.

For requests with the same priority, insertion order is preserved.

Example:

```text
Request 1 → NORMAL
Request 2 → HIGH
Request 3 → LOW
Request 4 → HIGH
Request 5 → NORMAL
```

Possible dequeue order:

```text
2 → 4 → 1 → 5 → 3
```

---

### Dynamic Batching

Instead of processing every request individually, InferX can combine requests into batches.

A `DynamicBatcher` currently supports two batch-readiness conditions:

1. the configured maximum batch size is reached
2. the configured waiting timeout is reached

Example with a maximum batch size of three:

```text
Requests:

1 2 3 4 5 6

        ↓

Batch 1
[1 2 3]

Batch 2
[4 5 6]
```

This models an important idea used by inference-serving systems: balancing batching efficiency against request waiting time.

---

### Thread-Safe Queue

Batches are transferred to workers using a thread-safe queue.

The queue coordinates producer and consumer activity and supports safe shutdown of waiting worker threads.

This component provides synchronization between batch creation and concurrent execution.

---

### Concurrent Worker Pool

InferX uses a worker pool built with C++ threads.

Multiple workers can consume batches concurrently.

For example:

```text
              Batch Queue
                  |
          +-------+-------+
          |               |
          v               v
      Worker 1         Worker 2
          |               |
          v               v
       Batch A          Batch B
```

Batch processing time is currently simulated using timed waits.

This allows InferX to demonstrate and measure concurrent execution without requiring an actual inference runtime.

---

## Performance Metrics

InferX v0.7 introduced a thread-safe metrics collector.

When requests complete, worker threads record their completion timestamps.

The metrics engine currently calculates:

- number of requests processed
- number of batches processed
- average request latency
- P50 latency
- P95 latency
- P99 latency
- throughput in requests per second
- average batch size

Example output from a small functional run:

```text
=====================================
      InferX Performance Report
=====================================
Requests processed : 6
Batches processed  : 2
Average latency    : <measured at runtime>
P50 latency        : <measured at runtime>
P95 latency        : <measured at runtime>
P99 latency        : <measured at runtime>
Throughput         : <measured at runtime>
Average batch size : 3.00
=====================================
```

Latency and throughput values vary between runs because they are measured from actual program execution.

The current six-request workload is intended as a functional demonstration and should **not** be interpreted as a formal performance benchmark.

---

## Automated Testing

InferX uses **GoogleTest** for automated testing.

The current test suite contains **8 tests** covering four major components.

### FIFO Scheduler

Tests verify:

- insertion order is preserved
- dequeuing from an empty scheduler throws an error

### Priority Scheduler

Tests verify:

- higher-priority requests are processed first
- insertion order is preserved for equal-priority requests

### Dynamic Batcher

Tests verify:

- a batch becomes ready when maximum batch size is reached
- a partially filled batch becomes ready after its timeout

### Metrics Engine

Tests verify:

- request and batch statistics are recorded
- percentile calculations produce the expected results

Current test result:

```text
100% tests passed, 0 tests failed out of 8
```

The tests are integrated with CMake and CTest.

---

## Technology Stack

| Area | Technology |
|---|---|
| Language | C++20 |
| Build System | CMake |
| Compiler | MSVC |
| Concurrency | `std::thread`, mutexes, atomics |
| Synchronization | Thread-safe queue |
| Data Structures | STL containers and queues |
| Testing | GoogleTest |
| Test Runner | CTest |
| Version Control | Git / GitHub |

---

## Project Structure

```text
inferx/
│
├── include/
│   └── inferx/
│       ├── inference_request.hpp
│       ├── fifo_scheduler.hpp
│       ├── priority_scheduler.hpp
│       ├── batch.hpp
│       ├── dynamic_batcher.hpp
│       ├── thread_safe_queue.hpp
│       ├── worker_pool.hpp
│       └── metrics.hpp
│
├── src/
│   ├── main.cpp
│   ├── inference_request.cpp
│   ├── fifo_scheduler.cpp
│   ├── priority_scheduler.cpp
│   ├── batch.cpp
│   ├── dynamic_batcher.cpp
│   ├── worker_pool.cpp
│   └── metrics.cpp
│
├── tests/
│   ├── test_fifo_scheduler.cpp
│   ├── test_priority_scheduler.cpp
│   ├── test_dynamic_batcher.cpp
│   └── test_metrics.cpp
│
├── benchmarks/
├── docs/
│
├── CMakeLists.txt
├── .gitignore
└── README.md
```

The `benchmarks/` and `docs/` directories are reserved for later project milestones.

---

## Building InferX

### Requirements

The current Windows development environment uses:

- CMake
- Visual Studio Build Tools 2022
- MSVC C++ compiler
- Git

GoogleTest is obtained automatically through CMake `FetchContent`.

---

### Clone the Repository

```bash
git clone https://github.com/Rajkumar0863/inferx.git
cd inferx
```

---

### Configure

From a Visual Studio x64 Native Tools Command Prompt:

```bash
cmake -S . -B build
```

CMake will configure the InferX targets and fetch GoogleTest when required.

---

### Build

```bash
cmake --build build
```

For the default Visual Studio Debug configuration, the executable is generated under:

```text
build/Debug/inferx.exe
```

---

### Run InferX

On Windows:

```bash
build\Debug\inferx.exe
```

A successful run displays worker execution followed by the performance report.

---

## Running the Tests

First configure and build the project:

```bash
cmake -S . -B build
cmake --build build
```

Then run:

```bash
ctest --test-dir build -C Debug --output-on-failure
```

At the v0.8 milestone, the expected result is:

```text
100% tests passed, 0 tests failed out of 8
```

---

## Development Milestones

### v0.1 — Project Foundation

- C++20 project structure
- CMake build configuration
- initial executable

### v0.2 — FIFO Scheduling

- request queue
- FIFO scheduling behaviour

### v0.3 — Priority Scheduling

- HIGH / NORMAL / LOW priorities
- stable ordering for equal priorities

### v0.4 — Dynamic Batching

- batch abstraction
- configurable batch size
- timeout-based batch readiness

### v0.5 / v0.6 — Concurrent Execution

- thread-safe queue
- worker pool
- concurrent batch processing
- controlled worker shutdown

### v0.7 — Performance Metrics

- request latency tracking
- average latency
- P50 / P95 / P99 latency
- throughput
- batch statistics
- thread-safe metric collection

### v0.8 — Automated Testing

- GoogleTest integration
- CTest integration
- FIFO scheduler tests
- priority scheduler tests
- dynamic batching tests
- metrics tests
- 8 automated tests passing

---

## Planned Work

The next development stages are planned to include:

- benchmark harness
- larger synthetic workloads
- configurable worker counts
- configurable batch sizes
- repeatable performance experiments
- CSV benchmark export
- comparison of scheduling configurations
- additional edge-case and concurrency tests
- GitHub Actions CI
- expanded architecture documentation

Potential later extensions may include:

- request cancellation
- queue backpressure
- model-aware scheduling
- starvation prevention
- load generation
- additional scheduling policies

These features are roadmap items and are **not yet implemented**.

---

## Benchmarking Status

Formal benchmarking has not yet been completed.

The current performance report measures real execution timing, but the existing six-request workload is primarily intended to validate the scheduler, worker pool, and metrics pipeline.

Future benchmark work will evaluate larger workloads and compare configurations such as:

```text
Requests:
100
1,000
10,000

Worker counts:
1
2
4
8

Batch sizes:
1
2
4
8
16
```

Results will only be documented after they have been measured experimentally.

---

## What This Project Demonstrates

InferX currently provides hands-on implementation experience with:

- modern C++20
- object-oriented design
- STL data structures
- scheduling algorithms
- priority queues
- batching strategies
- multithreading
- synchronization
- thread-safe shared state
- worker-pool architecture
- latency measurement
- percentile statistics
- throughput measurement
- automated unit testing
- CMake-based C++ project organization
- Git-based incremental development

---

## Scope

InferX is an educational systems-engineering simulator.

It is **not** currently:

- a production inference server
- a GPU scheduler
- a CUDA runtime
- a distributed inference platform
- a replacement for production serving systems

The goal is to implement and understand the underlying scheduling, batching, concurrency, testing, and performance concepts before extending the project further.

---

## Author

**Rajkumar Vijayan**

MSc Software Development (International Systems)  
University of Limerick, Ireland

GitHub: [Rajkumar0863](https://github.com/Rajkumar0863)

---

## Current Status

**InferX v0.8**

```text
[✓] Request model
[✓] FIFO scheduling
[✓] Priority scheduling
[✓] Dynamic batching
[✓] Thread-safe queue
[✓] Concurrent worker pool
[✓] Performance metrics
[✓] GoogleTest
[✓] 8/8 automated tests passing

[ ] Benchmark harness
[ ] Large workload experiments
[ ] CSV benchmark export
[ ] GitHub Actions CI
```

Development will continue from the benchmark milestone.