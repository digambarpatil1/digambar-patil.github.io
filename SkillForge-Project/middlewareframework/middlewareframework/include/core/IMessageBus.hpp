#pragma once
#include "core/messageTypes.hpp" ///< Ensure Message.hpp contains the full struct definition, not just a forward declaration
#include <functional>

/**
 * @interface IMessageBus
 * @brief Abstract interface for a message bus system.
 *
 * Provides a publish/subscribe mechanism for Message objects.
 * Plugins and sensors use this interface to communicate asynchronously.
 */
class IMessageBus {
public:
    /// Type alias for a subscriber callback function
    using Callback = std::function<void(const Message&)>;

    /**
     * @brief Publish a message to all subscribers.
     *
     * @param msg The message to be published.
     */
    virtual void publish(const Message& msg) = 0;

    /**
     * @brief Subscribe a callback function to receive messages.
     *
     * Each subscriber will be called when a message is published.
     *
     * @param callback Function to be called for each published message.
     */
    virtual void subscribe(Callback callback) = 0;

    /**
     * @brief Virtual destructor for proper cleanup of derived classes.
     */
    virtual ~IMessageBus() = default;
};
