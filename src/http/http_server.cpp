#include "include/http_server.hpp"

namespace nyx {
namespace http_server {
IHttpServer::IHttpServer(std::string_view addr, size_t port) {}

void IHttpServer::start() {}
};  // namespace http_server
}  // namespace nyx
