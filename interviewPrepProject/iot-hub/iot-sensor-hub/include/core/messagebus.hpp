#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include "core/IMessageBus.hpp"
#include <functional>

class MessageBus: public IMessageBus {

public:

  void publish(const Message& msg) override;
  void subscribe(Callback callback) override; // Subscribe with a callback
    ~MessageBus() override = default;
  private:
    std::vector<Callback> subscribers;
    std::mutex mtx;
  
};