#ifndef CENTRALIZED_THREAD_POOL_HPP
#define CENTRALIZED_THREAD_POOL_HPP

#include <atomic>
#include <functional>
#include <future>

#include "src/data_structure/concurrent_queue.hpp"

namespace nyx::http::threadpool {

/**
 * @brief Type alias for a task function.
 */
typedef std::function<void()> Task;

/**
 * @brief Abstract base class for a thread pool.
 *
 * Manages worker threads and a queue of tasks to execute.
 */
class IThreadPool {
 public:
  /**
   * @brief Constructs a thread pool
   */
  IThreadPool() = default;

  /**
   * @brief Virtual destructor for cleanup.
   */
  virtual ~IThreadPool();

  /**
   * @brief Submits a task to the thread pool.
   *
   * @tparam F Function type.
   * @tparam Args Argument types.
   * @param f Function to execute.
   * @param args Arguments for the function.
   * @return A future representing the task's result.
   */
  template <typename F, typename... Args>
  auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;

  /**
   * @brief Gracefully shuts down the thread pool, stopping task execution.
   */
  virtual void shutdown();

  /**
   * @brief Waits until all tasks in the queue are completed.
   */
  virtual void wait_all();

  // Disable copying
  IThreadPool(const IThreadPool&) = delete;
  IThreadPool& operator=(const IThreadPool&) = delete;

 protected:
  std::vector<std::thread> workers;             ///< Worker threads.
  data_structure::ConcurrentQueue<Task> tasks;  ///< FIFO task queue.
  std::condition_variable completion_cond;      ///< Condition variable for waiting.
  std::atomic<bool> stop_requested = false;     ///< Indicates whether shutdown has been requested.

  /**
   * @brief Enqueues a task for execution.
   *
   * @param task The task to execute.
   */
  virtual void enqueue_task(std::function<void()> task);
};

/**
 * @brief A centralized thread pool implementation.
 *
 * Inherits from IThreadPool and manages worker threads that process tasks concurrently.
 */
class CentralizedPool : public IThreadPool {
 public:
  /**
   * @brief Constructs a thread pool with a specified number of threads.
   *
   * @param num_threads The number of worker threads.
   */
  explicit CentralizedPool(size_t num_threads);

  /**
   * @brief Destructor that ensures proper cleanup.
   */
  ~CentralizedPool() override;

  /**
   * @brief Submits a task to the centralized pool.
   *
   * @tparam F Function type.
   * @tparam Args Argument types.
   * @param f Function to execute.
   * @param args Arguments for the function.
   * @return A future representing the task's result.
   */
  template <typename F, typename... Args>
  auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;

  /**
   * @brief Shuts down the thread pool, preventing new tasks from being enqueued.
   */
  void shutdown() override;

  /**
   * @brief Waits for all tasks to complete before returning.
   */
  void wait_all() override;

 private:
  /**
   * @brief The main worker thread function that processes tasks from the queue.
   */
  void worker_thread();
};

}  // namespace nyx::http::threadpool

#endif  // CENTRALIZED_THREAD_POOL_HPP
