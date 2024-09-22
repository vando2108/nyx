#ifndef EVENT_KQUEUE_EVENT_CENTER_HPP
#define EVENT_KQUEUE_EVENT_CENTER_HPP

#include <mutex>

// #include "src/event/include/define.hpp"
// #include "src/event/include/event.hpp"
#include "src/event/include/event_monitor.hpp"

namespace nyx {
namespace event {
namespace kqueue {
class KqueueEventMonitor : public IEventMonitor {
 public:
  KqueueEventMonitor();
  ~KqueueEventMonitor();

 public:
  bool register_event(int, int) const override;

 private:
  // std::unordered_map<event_id_t, std::list<std::unique_ptr<IObserver>>> registered_event_;
  mutable std::mutex registered_event_mutex_;
};
}  // namespace kqueue
}  // namespace event

}  // namespace nyx

#endif  // !EVENT_KQUEUE_EVENT_CENTER_HPP
