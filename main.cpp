#include <glog/logging.h>
#include <netinet/in.h>
#include <sys/_endian.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <condition_variable>
#include <cstring>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

#include "src/data_structure/priority_queue.hpp"

// #include "data_structure/stealing_work_queue.hpp"
// #include "http/http_server.hpp"
// #include "socket/stream.hpp"
// #include "threadpool/base.hpp"
// #include "threadpool/stealing_threadpool.hpp"

std::mutex mutex;
std::condition_variable condition_variable;
bool lock_key;

// void thread(int id) {
//   std::unique_lock<std::mutex> lock{mutex};
//   condition_variable.wait(lock, [&] { return lock_key; });
//   LOG(INFO) << "thread " << id << " finish running";
// }

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  // Set log info to console
  FLAGS_alsologtostderr = 1;

  nyx::data_structure::priority_queue<int> pq;
  int x = 10;
  pq.push_and_update(std::move(x), 0);
  int y = 11;
  pq.push_and_update(std::move(y), 0);
  pq.remove(10);
  auto temp = pq.try_pop();
  if (temp.has_value()) {
    LOG(INFO) << temp.value();
  } else {
    LOG(INFO) << "error";
  }
  temp = pq.try_pop();
  if (temp.has_value()) {
    LOG(INFO) << temp.value();
  } else {
    LOG(INFO) << "error";
  }
  temp = pq.try_pop();
  if (temp.has_value()) {
    LOG(INFO) << temp.value();
  } else {
    LOG(INFO) << "error";
  }

  auto z = 12;
  pq.push_no_update(std::move(z), 63);

  if (pq.get_priority(12).has_value()) {
    LOG(INFO) << "priority: " << static_cast<int>(pq.get_priority(12).value());
  } else {
    LOG(INFO) << "pq error";
  }

  // nyx::socket::ServerStream server = nyx::socket::ServerStream("127.0.0.1", 3000);
  // server.start();

  // nyx::threadpool::Config config;
  // config.minimum_thread = 10;
  // auto threadpool = nyx::threadpool::stealing::StealingThreadpool::create(std::move(config));

  // nyx::data_structure::UniqueList<int> unique_list;
  //
  // LOG(INFO) << "test";

  // nyx::data_structure::StealingWorkQueue<int> tasks;
  //
  // for (;;) {
  //   char buff[1024];
  //   char* cmd = fgets(buff, sizeof(buff), stdin);
  //
  //   if (cmd == nullptr) {
  //     break;
  //   }
  // }
  //
  // LOG(INFO) << "terminate...";

  return 0;
}
