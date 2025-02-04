#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <stdexcept>
#include <utility>

#include "src/http/threadpool/include/threadpool.hpp"

namespace nyx::http::threadpool {
CentralizedPool::CentralizedPool(size_t num_threads) : IThreadPool() {
  for (size_t i = 0; i < num_threads; ++i) {
    workers.emplace_back([this] {
      while (true) {
        Task task;
        if (stop_requested) {
          // stop when stop_requested and there no task in the queue
          if (!tasks.try_pop(task)) {
            return;
          }
        } else {
        }
        if (stop_requested && tasks.try_pop(task)) return;
        task();
        completion_cond.notify_one();
      }
    });
  }
}

CentralizedPool::~CentralizedPool() {}

template <typename F, typename... Args>
auto CentralizedPool::submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
  using return_type = decltype(f(args...));

  auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> result = task->get_future();
  if (stop_requested) {
    throw std::runtime_error("sunmit() called on stopped CentralizedPool");
  }
  tasks.push([task]() { (*task)(); });

  return result;
}
}  // namespace nyx::http::threadpool
