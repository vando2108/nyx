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
root/
│
├── src/                  # Contains all source files
│   ├── data_structures/   # Implementations of data structures
│   ├── system_design/     # System design-related code
│   └── utils/             # Utility code shared across the project
│
├── include/              # Header files
│
├── tests/                # Unit tests for the project
│
├── bazel_script/         # Bazel build scripts
│
└── README.md             # This file
```

## Build Instructions

To build and run the project, follow these steps:

### Prerequisites

- **Bazel** (required to manage builds)
- **Google Test** (for running unit tests)
- A C++ compiler (e.g., g++)

### Clone the Repository

```bash
git clone https://github.com/yourusername/your-repo-name.git
cd your-repo-name
```

### Build the Project

To build all the targets, simply run:

```bash
bazel build //...
```

### Run Unit Tests

You can run the tests by executing:

```bash
bazel test //tests:all
```

This will run all tests defined in the `tests/` directory.

## Future Work

- Add more complex data structures and algorithms.
- Extend system design components to handle real-world scenarios.
- Further integrate work-inspired solutions for deep technical insight.
