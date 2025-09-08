#pragma once
#include "core/message.hpp" // Ensure Message.hpp contains the full struct definition, not just a forward declaration
#include <functional>
class IMessageBus {

    public:
    using Callback = std::function<void(const Message&)>; // Define a callback type
      virtual void publish(const Message& msg)=0;
      virtual void subscribe(Callback callback)=0; // Subscribe with a callback
      virtual ~IMessageBus()=default;
      
    };