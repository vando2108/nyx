# Makefile to build Bazel project, generate compile_commands.json, and run unit tests

# Default Bazel build options
BAZEL_BUILD_OPTS = --cxxopt=-std=c++20
BAZEL_TESTS_OPTS = --cxxopt=-std=c++20 --test_output=streamed --color=yes

# The target to build (can be overridden from the command line)
TARGET ?= //benchmark/data_structure:scsp_mutex_queue_benchmark

# The default goal is to build the specified target
.PHONY: all
all: build

# Build rule
.PHONY: build
build:
	bazel build $(BAZEL_BUILD_OPTS) $(TARGET)

# Rule to generate compile_commands.json using Hedron Compile Commands
.PHONY: compile_commands
compile_commands:
	bazel run @hedron_compile_commands//:refresh_all

# Clean rule to clean Bazel build outputs
.PHONY: clean
clean:
	bazel clean

# Rule to run Bazel tests
.PHONY: test
test:
	bazel test $(BAZEL_TESTS_OPTS) $(TARGET)

# Help rule to display usage information
.PHONY: help
help:
	@echo "Usage:"
	@echo "  make [TARGET=your_bazel_target] [OPTIONS]"
	@echo "Examples:"
	@echo "  make" 
	@echo "  make TARGET=//benchmark/data_structure:another_target"
	@echo "  make clean"
	@echo "  make compile_commands - To generate or refresh compile_commands.json"
	@echo "  make test - To run Bazel tests with specified options"
