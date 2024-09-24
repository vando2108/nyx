#include <benchmark/benchmark.h>

#include <optional>
#include <queue>

#include "src/data_structure/priority_queue.hpp"

// Benchmarking the push operation for your custom priority_queue
static void BM_CustomPriorityQueuePush(benchmark::State& state) {
  nyx::data_structure::priority_queue<int> pq;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      int x = i;
      pq.push_and_update(std::move(x), i % 64);
    }
  }
}
BENCHMARK(BM_CustomPriorityQueuePush)->Arg(1000)->Arg(10000)->Arg(100000);

// Benchmarking the push operation for std::priority_queue
static void BM_StdPriorityQueuePush(benchmark::State& state) {
  std::priority_queue<int> pq;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      pq.push(i);
    }
  }
}
BENCHMARK(BM_StdPriorityQueuePush)->Arg(1000)->Arg(10000)->Arg(100000);

// Benchmarking the pop operation for your custom priority_queue
static void BM_CustomPriorityQueuePop(benchmark::State& state) {
  nyx::data_structure::priority_queue<int> pq;
  for (int i = 0; i < state.range(0); ++i) {
    int x = i;
    pq.push_no_update(std::move(x), i % 8);
  }
  for (auto _ : state) {
    while (pq.try_pop().has_value());
  }
}
BENCHMARK(BM_CustomPriorityQueuePop)->Arg(1000)->Arg(10000)->Arg(100000);

// Benchmarking the pop operation for std::priority_queue
static void BM_StdPriorityQueuePop(benchmark::State& state) {
  std::priority_queue<int> pq;
  for (int i = 0; i < state.range(0); ++i) {
    pq.push(i);
  }
  for (auto _ : state) {
    while (!pq.empty()) {
      pq.pop();
    }
  }
}
BENCHMARK(BM_StdPriorityQueuePop)->Arg(1000)->Arg(10000)->Arg(100000);

// Main function for Google Benchmark
BENCHMARK_MAIN();
