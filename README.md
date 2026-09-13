\# InferX



\*\*InferX\*\* is a modern C++20 AI inference request scheduler simulator built to explore scheduling, dynamic batching, concurrency, and performance trade-offs in AI inference-serving systems.



The project is being developed as a systems-programming project with a focus on clean C++ design, multithreading, scheduling algorithms, automated testing, and performance measurement.



> \*\*Current status:\*\* Early development. The C++20/CMake project skeleton is being established first, with scheduling, batching, concurrency, testing, and benchmarking added incrementally.



\---



\## Motivation



Modern AI inference systems may receive many requests concurrently.



A serving system must make decisions such as:



\- Which request should execute next?

\- Should high-priority requests be served before normal requests?

\- Should multiple requests be grouped into a batch?

\- How long should the system wait for a batch to fill?

\- How many workers should process requests concurrently?

\- How do scheduling and batching decisions affect latency and throughput?



InferX provides a controlled simulator for experimenting with these ideas without requiring a real GPU or machine-learning model.



\---



\## Planned Architecture



```text

&#x20;                 Incoming Requests

&#x20;                        |

&#x20;                        v

&#x20;               +-----------------+

&#x20;               |  Request Queue  |

&#x20;               +-----------------+

&#x20;                        |

&#x20;                        v

&#x20;               +-----------------+

&#x20;               |    Scheduler    |

&#x20;               | FIFO / Priority |

&#x20;               +-----------------+

&#x20;                        |

&#x20;                        v

&#x20;               +-----------------+

&#x20;               | Dynamic Batcher |

&#x20;               +-----------------+

&#x20;                        |

&#x20;                        v

&#x20;             +---------------------+

&#x20;             | Concurrent Workers  |

&#x20;             +---------------------+

&#x20;                |       |       |

&#x20;                v       v       v

&#x20;               W1      W2      W3

&#x20;                 \\      |      /

&#x20;                  \\     |     /

&#x20;                   v    v    v

&#x20;               Simulated Inference

&#x20;                        |

&#x20;                        v

&#x20;               +-----------------+

&#x20;               | Metrics Engine  |

&#x20;               +-----------------+

&#x20;                        |

&#x20;                        v

&#x20;              Latency / Throughput

&#x20;                 P50 / P95 / P99

```



\---



\## Development Goals



InferX is planned to support:



\- Modern C++20

\- FIFO scheduling

\- Priority-based scheduling

\- Dynamic request batching

\- Configurable batch size

\- Configurable batching timeout

\- Concurrent worker threads

\- Thread-safe queues

\- Simulated inference workloads

\- Latency measurement

\- Throughput measurement

\- P50, P95 and P99 latency statistics

\- Scheduler benchmarking

\- Unit testing with GoogleTest

\- CMake-based builds



\---



\## Scheduling Strategies



\### FIFO



The FIFO scheduler will process requests in arrival order.



```text

R1 -> R2 -> R3 -> R4

```



This provides a simple baseline against which other scheduling strategies can be compared.



\### Priority Scheduling



Requests will be able to carry priorities such as:



```text

HIGH

NORMAL

LOW

```



A priority scheduler will select higher-priority requests before lower-priority requests while preserving deterministic behaviour between requests of equal priority.



Example:



```text

Arrival:



R1 LOW

R2 HIGH

R3 NORMAL

R4 HIGH



Expected priority order:



R2 -> R4 -> R3 -> R1

```



\---



\## Dynamic Batching



InferX will support grouping multiple compatible requests into execution batches.



For example:



```text

Maximum batch size: 4



Incoming:

R1 R2 R3 R4 R5 R6



Batches:



Batch 1:

R1 R2 R3 R4



Batch 2:

R5 R6

```



A batch will eventually be dispatched when either:



1\. The maximum configured batch size is reached, or

2\. The oldest eligible request reaches the configured waiting-time threshold.



This will allow InferX to explore the trade-off between throughput and request latency.



\---



\## Concurrency



A later development phase will introduce concurrent worker threads.



```text

&#x20;               Batch Queue

&#x20;                    |

&#x20;            +-------+-------+

&#x20;            |       |       |

&#x20;            v       v       v

&#x20;         Worker 1 Worker 2 Worker 3

```



The implementation is planned to use C++ concurrency primitives including:



\- `std::thread`

\- `std::mutex`

\- `std::condition\_variable`

\- `std::atomic`



\---



\## Performance Metrics



InferX will instrument completed requests and calculate metrics such as:



\### Latency



```text

latency = completion\_time - arrival\_time

```



\### Percentile Latency



Planned percentile statistics:



```text

P50

P95

P99

```



\### Throughput



```text

throughput = completed\_requests / elapsed\_time

```



reported in requests per second.



\### Batch Statistics



Planned statistics include:



\- Number of requests processed

\- Number of batches

\- Average batch size

\- End-to-end request latency



\---



\## Example Future Benchmark Format



The following demonstrates the intended output format. The values are illustrative and are \*\*not measured InferX results\*\*.



```text

========================================

&#x20;      InferX Performance Report

========================================



Scheduler       : Priority

Requests        : 10000

Workers         : 4

Maximum Batch   : 8



Throughput      : 842.31 requests/sec



Latency

\----------------------------------------

Average         : 17.32 ms

P50             : 12.47 ms

P95             : 35.88 ms

P99             : 51.06 ms



Average Batch   : 6.43



========================================

```



Actual benchmark results will be added only after the corresponding implementation is complete.



\---



\## Project Structure



```text

inferx/

|

|-- CMakeLists.txt

|-- README.md

|-- LICENSE

|-- .gitignore

|

|-- include/

|   `-- inferx/

|

|-- src/

|   `-- main.cpp

|

|-- tests/

|

|-- benchmarks/

|

`-- docs/

```



The structure will expand as each subsystem is implemented.



\---



\## Technology Stack



| Area | Technology |

|---|---|

| Language | C++20 |

| Build System | CMake |

| Testing | GoogleTest (planned) |

| Concurrency | C++ Standard Library (planned) |

| Performance Measurement | `std::chrono` (planned) |

| Version Control | Git / GitHub |



\---



\## Build Instructions



\### Requirements



You need:



\- A C++20-compatible compiler

\- CMake 3.20 or later

\- Git



Examples of suitable compilers include recent versions of GCC, Clang, or Microsoft Visual C++.



Check CMake:



```bash

cmake --version

```



Check GCC if you are using it:



```bash

g++ --version

```



\---



\### Clone



```bash

git clone https://github.com/Rajkumar0863/inferx.git

cd inferx

```



\### Configure



```bash

cmake -S . -B build

```



\### Build



```bash

cmake --build build

```



\### Run



For a typical Visual Studio multi-configuration build on Windows:



```text

build\\Debug\\inferx.exe

```



For a single-configuration build, the executable may instead be located at:



```text

build\\inferx.exe

```



\---



\## Development Roadmap



\### Phase 1 - Project Foundation



\- \[x] Create GitHub repository

\- \[x] Define project scope

\- \[x] Create README

\- \[x] Configure C++20 with CMake

\- \[ ] Verify local build

\- \[ ] Create request model



\### Phase 2 - Scheduling



\- \[ ] Implement FIFO scheduler

\- \[ ] Implement priority scheduler

\- \[ ] Test scheduling behaviour



\### Phase 3 - Dynamic Batching



\- \[ ] Implement batch representation

\- \[ ] Add configurable maximum batch size

\- \[ ] Add batching timeout

\- \[ ] Test batching behaviour



\### Phase 4 - Concurrency



\- \[ ] Implement worker pool

\- \[ ] Implement thread-safe queues

\- \[ ] Add graceful shutdown

\- \[ ] Validate concurrent processing



\### Phase 5 - Metrics



\- \[ ] Record request latency

\- \[ ] Calculate average latency

\- \[ ] Calculate P50

\- \[ ] Calculate P95

\- \[ ] Calculate P99

\- \[ ] Calculate throughput

\- \[ ] Record batch statistics



\### Phase 6 - Testing and Benchmarking



\- \[ ] Integrate GoogleTest

\- \[ ] Create unit tests

\- \[ ] Create integration tests

\- \[ ] Generate configurable workloads

\- \[ ] Compare scheduling strategies

\- \[ ] Compare batch sizes

\- \[ ] Compare worker counts

\- \[ ] Record reproducible benchmark results



\### Phase 7 - Documentation



\- \[ ] Add architecture documentation

\- \[ ] Document design decisions

\- \[ ] Add benchmark results

\- \[ ] Document limitations

\- \[ ] Add usage examples



\---



\## Planned Experiments



Once the simulator is complete, InferX will be used to investigate questions such as:



\### FIFO vs Priority



How does priority scheduling affect latency for high-priority requests compared with FIFO?



\### Worker Count



How does throughput change when using:



```text

1 worker

2 workers

4 workers

8 workers

```



\### Batch Size



How does performance change with:



```text

Batch size 1

Batch size 2

Batch size 4

Batch size 8

Batch size 16

```



\### Batching Timeout



How does waiting longer for a batch to fill affect throughput and tail latency?



\---



\## Limitations



InferX is intended as a scheduler simulator rather than a production AI inference runtime.



The initial versions will not execute real neural-network or GPU inference. Instead, inference execution will be simulated using configurable processing times.



This allows the project to focus specifically on:



\- scheduling

\- batching

\- concurrency

\- synchronization

\- performance measurement



without requiring specialised AI hardware.



\---



\## Future Extensions



Possible future extensions include:



\- Deadline-aware scheduling

\- Weighted priorities

\- Adaptive batching

\- Request cancellation

\- Backpressure

\- Rate limiting

\- Multiple model queues

\- Per-model batching

\- Work stealing

\- JSON configuration

\- Trace-file workload replay

\- ONNX Runtime integration

\- Real model inference

\- Metrics export



These are future ideas rather than current project features.



\---



\## Learning Objectives



InferX is being built as a practical exploration of:



\- Modern C++

\- Object-oriented design

\- Data structures

\- STL containers

\- Scheduling algorithms

\- Multithreading

\- Synchronisation

\- Producer-consumer systems

\- CMake

\- Automated testing

\- Performance measurement

\- Software architecture

\- Technical documentation



\---



\## Author



\*\*Rajkumar Vijayan\*\*



MSc Software Development  

University of Limerick, Ireland



GitHub: Rajkumar0863



\---



\## License



This project is released under the MIT License.

