#include <benchmark/benchmark.h>
#include <glog/logging.h>

#include <chrono>
#include <queue>
#include <unordered_map>

#include "src/data_structure/priority_queue.hpp"
#include "src/utils/include/rand.hpp"

namespace nyx::benchmark {
static void BM_PushNoUpdate(::benchmark::State& state) {
  for (auto _ : state) {
    data_structure::priority_queue<size_t> pq;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < state.range(0); ++i) {
      size_t x = i;
      pq.push_no_update(std::move(x), 0);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}
// BENCHMARK(BM_PushNoUpdate)->RangeMultiplier(10)->Range(1'000, 1'000'000)->UseManualTime();

static void BM_PushAndUpdate(::benchmark::State& state) {
  for (auto _ : state) {
    data_structure::priority_queue<size_t> pq;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < state.range(0); ++i) {
      size_t x = i;
      pq.push_and_update(std::move(x), 0);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}
// BENCHMARK(BM_PushAndUpdate)->RangeMultiplier(10)->Range(1'000, 1'000'000)->UseManualTime();

static void BM_unordered_map(::benchmark::State& state) {
  nyx::data_structure::priority_queue<size_t>::addr_map_t mp(state.range());

  for (int i = 0; i < state.range(0); ++i) {
    mp[i] = std::make_pair(-1, nyx::data_structure::priority_queue<size_t>::node_addr_t());
  }

  for (auto _ : state) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < state.range(0); ++i) {
      mp.find(i);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}
BENCHMARK(BM_unordered_map)->RangeMultiplier(10)->Range(1'000, 1'000'000)->UseManualTime();
}  // namespace nyx::benchmark

BENCHMARK_MAIN();
