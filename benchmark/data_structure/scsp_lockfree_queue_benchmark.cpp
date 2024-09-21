#include <benchmark/benchmark.h>

#include <mutex>

#include "src/data_structure/scsp_lockfree_queue.hpp"

using namespace nyx::data_structure;

class ScspLockFreeQueueFixture : public ::benchmark::Fixture {
 public:
  static nyx::data_structure::ScspLockFreeQueue<int> queue;
  static std::mutex mu;

  void SetUp(::benchmark::State& state) override {
    std::lock_guard<std::mutex> lock(mu);
    const int64_t number_of_elements = state.range(0);
    if (number_of_elements != queue.capacity()) {
      ScspLockFreeQueueFixture::queue = std::move(nyx::data_structure::ScspLockFreeQueue<int>(number_of_elements));
    }
  }

  void TearDown(::benchmark::State& state) override {}
};

BENCHMARK_DEFINE_F(ScspLockFreeQueueFixture, ScspLockFreeQueueBenchmark)(::benchmark::State& state) {
  const bool is_push_thread = state.thread_index() == 0;
  const int64_t number_of_elements = state.range(0);

  if (is_push_thread) {
    for (auto _ : state) {
      for (int64_t i = 0; i < number_of_elements; ++i) {
        queue.push(i);
      }
    }
  } else {
    int value = -1;
    for (auto _ : state) {
      for (int64_t i = 0; i < number_of_elements; ++i) {
        queue.pop(value);
      }
    }
  }
}

ScspLockFreeQueue<int> ScspLockFreeQueueFixture::queue(1000);
std::mutex ScspLockFreeQueueFixture::mu;

BENCHMARK_REGISTER_F(ScspLockFreeQueueFixture, ScspLockFreeQueueBenchmark)->Range(1000, 1000000)->Threads(2);
BENCHMARK_MAIN();
