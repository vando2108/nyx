#include <glog/logging.h>
#include <netinet/in.h>
#include <sys/_endian.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>

#include "src/data_structure/concurrent_queue.hpp"

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  // Set log info to console
  FLAGS_alsologtostderr = 1;

  // nyx::data_structure::priority_queue<int> pq;
  // int x = 10;
  // pq.push_no_update(std::move(x), 10);
  // x = 10;

  nyx::data_structure::ConcurrentQueue<int> queue;
  for (int i = 0; i < 10; ++i) {
    queue.try_push(i);
    LOG(INFO) << "pushed: " << i << '\n';
  }

  int temp;
  while (queue.try_pop(temp)) {
    LOG(INFO) << "poped value: " << temp;
  }

  // pq.push_and_update(std::move(x), 9);
  // auto popped = pq.try_pop();
  // LOG(INFO) << static_cast<int>(popped.value());

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
