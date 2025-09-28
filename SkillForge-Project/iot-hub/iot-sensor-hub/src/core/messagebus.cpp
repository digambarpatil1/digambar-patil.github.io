#include "core/messagebus.hpp"

void MessageBus::publish(const Message& msg) {
    //std::lock_guard<std::mutex> lock(mtx);
    auto current = this->subscribers.load(); // Atomic load of the shared_ptr to the vector of callbacks 
    if (!current) return; // No subscribers
    // Notify all subscribers   
    for (const auto& cb : *current) {
        cb(msg); // Notify all subscribers
    }
}
void  MessageBus::subscribe(Callback callback) {
   // std::lock_guard<std::mutex> lock(mtx);
   using shared_ptr_vec = std::shared_ptr<std::vector<Callback>>;
   shared_ptr_vec old_subscribers;
   shared_ptr_vec new_subscribers;
    do
    {
        old_subscribers = subscribers.load(); // Atomic load of the current subscribers
        new_subscribers = std::make_shared<std::vector<Callback>>(); // Create a new vector
        if (old_subscribers) {
            *new_subscribers = *old_subscribers; // Copy old subscribers to new vector
        }
        new_subscribers->push_back(callback); // Add the new subscriber
    } while (!subscribers.compare_exchange_weak(old_subscribers, new_subscribers));
    // Loop until we successfully update the subscribers atomically
}
//Thread-safe using std::mutex. Later, we can optimize with a lock-free queue.