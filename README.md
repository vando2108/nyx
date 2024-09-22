# Nyx C++ Project

## Overview

This project serves as a collection of C++ implementations of various **data structures**, **system designs**, and **technical challenges** I've encountered during my job. My primary goal is to gain a deeper understanding of how these techniques work under the hood by implementing them from scratch.

### Key Objectives

- **Data Structures**: Implement core data structures (e.g., lists, trees, graphs, etc.) to strengthen my understanding of their mechanics.
- **System Design**: Break down complex systems into simpler components and simulate their design and interactions.
- **Practical Applications**: Re-implement technical solutions to challenges encountered in my job, helping me understand their behavior and performance.

By implementing everything from the ground up, I aim to reinforce theoretical knowledge with practical experience.

## Features

### Data structures

- **scsp_mutex_queue**: Done
- **scsp_lockfree_queue**: Done
- **unique_list**: Done
- **stealing_work_queue**: Test and benchmark
- **lockfree_growing_circular_array**: WIP

## Project Structure

```
nyx/
├── compile_commands.json
├── cpplint.cfg
├── docs
│   ├── *.md
├── main.cpp
├── src
│   ├── common
│   │   ├── BUILD.bazel
│   │   └── include
│   │       └── define.hpp
│   ├── data_structure
│   │   ├── BUILD.bazel
│   │   ├── lockfree_growing_circular_array.hpp
│   │   ├── scsp_lockfree_queue.hpp
│   │   ├── scsp_mutex_queue.hpp
│   │   ├── stealing_work_queue.hpp
│   │   └── unique_list.hpp
│   ├── http
│   │   ├── BUILD.bazel
│   │   ├── event
│   │   │   ├── BUILD.bazel
│   │   │   ├── include
│   │   │   │   ├── define.hpp
│   │   │   │   ├── event.hpp
│   │   │   │   └── event_monitor.hpp
│   │   │   └── kqueue
│   │   │       ├── BUILD.bazel
│   │   │       ├── event_monitor.cpp
│   │   │       └── include
│   │   │           └── event_monitor.hpp
│   │   ├── http_server.cpp
│   │   ├── include
│   │   │   └── http_server.hpp
│   │   ├── socket
│   │   │   ├── BUILD.bazel
│   │   │   ├── client_stream.cpp
│   │   │   ├── helper.cpp
│   │   │   ├── include
│   │   │   │   ├── helper.hpp
│   │   │   │   └── stream.hpp
│   │   │   └── server_stream.cpp
│   │   └── threadpool
│   │       ├── BUILD.bazel
│   │       ├── centralized
│   │       │   ├── threadpool.cpp
│   │       │   └── worker.cpp
│   │       ├── include
│   │       │   ├── base.hpp
│   │       │   ├── centralized_threadpool.hpp
│   │       │   └── stealing_threadpool.hpp
│   │       └── stealing
│   │           ├── threadpool.cpp
│   │           └── worker.cpp
│   └── utils
│       ├── BUILD.bazel
│       ├── bitwise.cpp
│       ├── include
│       │   ├── bitwise.hpp
│       │   ├── rand.hpp
│       │   └── time.hpp
│       ├── rand.cpp
│       └── time.cpp
└── test
    ├── data_structure
    │   ├── BUILD.bazel
    │   ├── scsp_lockfree_queue_tests.cpp
    │   ├── stealing_work_queue_tests.cpp
    │   └── unique_list_tests.cpp
    ├── threadpool
    │   ├── BUILD.bazel
    │   └── centralized_threadpool_tests.cpp
    └── utils
        ├── BUILD.bazel
        └── bitwise_tests.cpp
```

## Build Instructions

To build and run the project, follow these steps:

### Prerequisites

- **Bazel** (required to manage builds)
- **Google Test** (for running unit tests)
- **Google log** (for logging)
- **Google benchmark** (for benchmark)
- **C++ 20**
- **Make** (for build scripts)

### Clone the Repository

```bash
git clone git@github.com:vando2108/nyx.git
cd nyx
```

### Build the Project

To build specific target, you can run:

```bash
make build TARGET=target_you_wanna_build
```

All targets are tagged based on their folder structure using a dynamic tagging function. The function **_create_tags()_** automatically generates tags by splitting the package path into components, where the **_create_tags()_** function retrieves the relative path from the workspace root to the folder containing the BUILD.bazel file. For example, a target located in src/http/event will be assigned the tags **_['all', 'src', 'http', 'event']_**. These tags enable efficient filtering and selective building of targets based on their directory hierarchy, using Bazel's --build_tag_filter or other query options.

```bash
make build_tags TAGS=http (To build all target in http folder).
make build_tags TAGS=src|benchmark (To build all sources and benchmarks).
make build_tags TAGS=all (To build all project).
```

### Run Unit Tests

You can run all tests by executing:

```bash
make test TARGET=//test/...
```

This will run all tests defined in the `tests/` directory.

Or you can run tests for specific library by executing:

```bash
make test TARGET=//test/utils:all
```

### Run Unit Tests

To generate compile_commands, you can executing:

```bash
make compile_commands
```

## Future Work

- Add more complex data structures and algorithms.
- Extend system design components to handle real-world scenarios.
- Further integrate work-inspired solutions for deep technical insight.
