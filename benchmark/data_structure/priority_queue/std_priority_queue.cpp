#include <benchmark/benchmark.h>

#include <queue>

#include "src/utils/include/rand.hpp"

namespace nyx::benchmark {
static void BM_Push(::benchmark::State& state) {
  for (auto _ : state) {
    std::priority_queue<size_t> pq;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < state.range(0); ++i) {
      size_t x = nyx::utils::rand::Xorshift::next();
      pq.push(x);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}
BENCHMARK(BM_Push)->RangeMultiplier(10)->Range(1'000, 1'000'000)->UseManualTime();
}  // namespace nyx::benchmark

BENCHMARK_MAIN();
