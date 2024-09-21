#include <string_view>

namespace nyx {
namespace http_server {

class IHttpServer {
 public:
  IHttpServer(std::string_view, size_t);

 public:
  virtual void start();
};

class HttpServer {};

}  // namespace http_server
}  // namespace nyx
