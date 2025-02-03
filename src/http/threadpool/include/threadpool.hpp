#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  explicit ThreadPool(size_t num_threads);
  virtual ~ThreadPool();

  // Submit a task (no priority)
  template <typename F, typename... Args>
  auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;

  // Gracefully shutdown the pool
  virtual void shutdown();

  // Wait until all tasks are completed
  virtual void wait_all();

  // Disable copying
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

 protected:
  std::vector<std::thread> workers;         // Worker threads
  std::queue<std::function<void()>> tasks;  // FIFO task queue
  std::mutex queue_mutex;                   // Protects the task queue
  std::condition_variable condition;        // For task availability
  std::condition_variable completion_cond;  // For wait_all()
  bool stop_requested = false;

  // Enqueue a task (implementation-specific)
  virtual void enqueue_task(std::function<void()> task);
};
