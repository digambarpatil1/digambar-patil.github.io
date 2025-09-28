#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include "core/IMessageBus.hpp"
#include <functional>
#include <vector>
#include <atomic>

/**
 * @class MessageBus
 * @brief Thread-safe implementation of IMessageBus using atomic shared_ptr for subscribers.
 *
 * Supports multiple subscribers and allows publishing messages asynchronously.
 * Internally uses a shared_ptr to a vector of callbacks to allow lock-free iteration
 * during publish, while subscribe updates are done atomically.
 */
class MessageBus : public IMessageBus {
public:
    /**
     * @brief Publish a message to all subscribers.
     *
     * Iterates over all subscribed callbacks and invokes them with the given message.
     *
     * @param msg The message to be published.
     */
    void publish(const Message& msg) override;

    /**
     * @brief Subscribe a callback function to receive published messages.
     *
     * The callback will be invoked each time a message is published.
     *
     * @param callback Function to call when a message is published.
     */
    void subscribe(Callback callback) override;

private:
    std::shared_ptr<std::vector<std::function<void(const Message&)>>> subscribers; ///< List of subscriber callbacks
    std::mutex mtx; ///< Mutex used for thread-safe updates to subscribers (if needed)
};
