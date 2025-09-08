#include "core/messagebus.hpp"

void MessageBus::publish(const Message& msg) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& cb : subscribers) {
        cb(msg); // Notify all subscribers
    }
}
void  MessageBus::subscribe(Callback callback) {
    std::lock_guard<std::mutex> lock(mtx);
    subscribers.push_back(callback);
}
//Thread-safe using std::mutex. Later, we can optimize with a lock-free queue.