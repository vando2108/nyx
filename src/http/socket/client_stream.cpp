#include <glog/logging.h>
#include <sys/socket.h>

#include "src/socket/include/stream.hpp"

namespace nyx {
namespace socket {
ClientStream::ClientStream(int fd) { set_fd(fd); }

long ClientStream::recv(std::vector<char>& buffer, int flags) const {
  if (fd() < 0) {
    LOG(FATAL) << "Client stream has invalid fd";
  }

  return ::recv(fd(), buffer.data(), buffer.size(), flags);
}
}  // namespace socket
}  // namespace nyx
