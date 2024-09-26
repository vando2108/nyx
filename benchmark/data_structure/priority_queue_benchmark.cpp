#include <_stdlib.h>
#include <benchmark/benchmark.h>

#include <cassert>
#include <queue>
#include <type_traits>

#include "src/data_structure/priority_queue.hpp"
#include "src/utils/include/rand.hpp"

#define random() nyx::utils::rand::Xorshift::next()

const static int MULTIPLIER = 10;
const static int RANGES[] = {1'000, 1'000'000};

// static void BM_Nyx_PriorityQueue_PushAndUpdate(benchmark::State& state) {
//   nyx::data_structure::priority_queue<int> pq;
//   for (auto _ : state) {
//     for (int i = 0; i < state.range(0); ++i) {
//       int x = i;
//       pq.push_and_update(std::move(x), i % 64);
//     }
//   }
// }
// BENCHMARK(BM_Nyx_PriorityQueue_PushAndUpdate)->RangeMultiplier(MULTIPLIER)->Range(RANGES[0], RANGES[1]);
//
// static void BM_Nyx_PriorityQueue_PushNoUpdate(benchmark::State& state) {
//   nyx::data_structure::priority_queue<int> pq;
//   for (auto _ : state) {
//     for (int i = 0; i < state.range(0); ++i) {
//       int x = i;
//       pq.push_no_update(std::move(x), i % 64);
//     }
//   }
// }
// BENCHMARK(BM_Nyx_PriorityQueue_PushNoUpdate)->RangeMultiplier(MULTIPLIER)->Range(RANGES[0], RANGES[1]);
//
// static void BM_Std_PriorityQueue_Push(benchmark::State& state) {
//   std::priority_queue<int> pq;
//   for (auto _ : state) {
//     for (int i = 0; i < state.range(0); ++i) {
//       pq.push(i);
//     }
//   }
// }
// BENCHMARK(BM_Std_PriorityQueue_Push)->RangeMultiplier(MULTIPLIER)->Range(RANGES[0], RANGES[1]);

template <typename T>
T setup_for_pop(const benchmark::State& state) {
  T pq;
  for (int i = 0; i < state.range(0); ++i) {
    size_t x = nyx::utils::rand::Xorshift::next();
    if constexpr (std::is_same<T, std::priority_queue<size_t>>::value) {
      pq.push(x);
    } else {
      pq.push_no_update(std::move(x), 0);
    }
  }

  return pq;
}

static void BM_Nyx_PriorityQueue_Pop(benchmark::State& state) {
  auto pq = setup_for_pop<nyx::data_structure::priority_queue<size_t>>(state);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      pq.try_pop();
    }
  }
}
BENCHMARK(BM_Nyx_PriorityQueue_Pop)->RangeMultiplier(MULTIPLIER)->Range(RANGES[0], RANGES[1]);

static void BM_Std_PriorityQueue_Pop(benchmark::State& state) {
  auto pq = setup_for_pop<std::priority_queue<size_t>>(state);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      pq.pop();
    }
  }
}
BENCHMARK(BM_Std_PriorityQueue_Pop)->RangeMultiplier(MULTIPLIER)->Range(RANGES[0], RANGES[1]);

static void BM_Nyx_PriorityQueue_PushPop(benchmark::State& state) {
  nyx::data_structure::priority_queue<int> pq;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      auto x = random(), y = random(), z = random() % 64;
      if (x & 1) {
        pq.push_no_update(y, 0);
      } else {
        pq.try_pop();
      }
    }
  }
}
BENCHMARK(BM_Nyx_PriorityQueue_PushPop)->RangeMultiplier(MULTIPLIER)->Range(RANGES[0], RANGES[1]);

static void BM_Std_PriorityQueue_PushPop(benchmark::State& state) {
  std::priority_queue<int> pq;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      auto x = random(), y = random(), _ = random() % 64;
      if (x & 1) {
        pq.push(y);
      } else {
        if (!pq.empty()) {
          pq.pop();
        }
      }
    }
  }
}
BENCHMARK(BM_Std_PriorityQueue_PushPop)->RangeMultiplier(MULTIPLIER)->Range(RANGES[0], RANGES[1]);

//-----------------

// Main function for Google Benchmark
BENCHMARK_MAIN();
