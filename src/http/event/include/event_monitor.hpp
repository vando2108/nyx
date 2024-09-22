#ifndef EVENT_EVENT_MONITOR_HPP
#define EVENT_EVENT_MONITOR_HPP

#include <sys/event.h>

#include "./event.hpp"

namespace nyx {
namespace event {
class IEventMonitor : public IEvent {
 public:
  ~IEventMonitor() {}

 public:
  virtual bool register_event(int, int) const = 0;
};
}  // namespace event
}  // namespace nyx

#endif  // !EVENT_EVENT_MONITOR_HPP
