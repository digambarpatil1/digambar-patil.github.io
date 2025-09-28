#include "core/messagebus.hpp"

/**
 * @brief Publish a message to all subscribers.
 *
 * This method atomically loads the current list of subscribers
 * and invokes each registered callback with the provided message.
 * If no subscribers are present, the function returns immediately.
 *
 * @param msg The message to publish.
 */
void MessageBus::publish(const Message& msg) {
    // Atomically load the shared pointer to the subscribers vector
    auto current = std::atomic_load(&subscribers);

    if (!current) return; // No subscribers registered

    // Notify all subscribers
    for (const auto& cb : *current) {
        cb(msg);
    }
}

/**
 * @brief Subscribe a new callback to the message bus.
 *
 * This method uses lock-free compare-and-swap to safely update
 * the list of subscribers in a thread-safe manner. Each new subscriber
 * is added to a copy of the current list, which is then atomically
 * swapped in.
 *
 * @param callback The callback function to be invoked on message publish.
 */
void MessageBus::subscribe(Callback callback) {
    using shared_ptr_vec = std::shared_ptr<std::vector<Callback>>;

    shared_ptr_vec old_subscribers;
    shared_ptr_vec new_subscribers;

    do {
        // Atomically load the current subscribers
        old_subscribers = std::atomic_load(&subscribers);

        // Create a new vector of subscribers
        new_subscribers = std::make_shared<std::vector<Callback>>();
        if (old_subscribers) {
            *new_subscribers = *old_subscribers; // Copy existing subscribers
        }

        // Add the new subscriber
        new_subscribers->push_back(callback);

        // Attempt to atomically swap the old subscriber list with the new one
    } while (!std::atomic_compare_exchange_weak(
        &subscribers,
        &old_subscribers,
        new_subscribers));

    // Loop continues until update succeeds
}

// Note: Thread safety is achieved using atomic operations.
// No mutex is required here, but this could be extended later
// with a lock-free queue for even higher performance.
