#include <gtest/gtest.h>
#include "core/messagebus.hpp"
#include "core/IMessageBus.hpp"
#include "core/messageTypes.hpp"
#include <any>
#include <string>
#include <atomic>

// A dummy test payload type
struct TestMessage {
    int value;
};
class MessageBusTestFixture : public ::testing::Test {
    protected:
        MessageBus bus;
        int receivedValue = 0;
    
        void SetUp() override {
            // Common setup before each test
            receivedValue = 0;
        }
    
        void TearDown() override {
            // Clean up if needed
        }
    };
    
    TEST_F(MessageBusTestFixture, SingleSubscriberReceivesMessage) {
        bus.subscribe([&](const Message& msg) {
            auto testMsg = std::any_cast<nlohmann::json>(msg.payload);
            receivedValue = testMsg["value"].get<int>();
        });
    
        TestMessage m{42};
        nlohmann::json pl = { {"value", 42} }; // test payload
        Message wrapped("sensor1", "test_topic", pl);
        bus.publish(wrapped);
    
        EXPECT_EQ(receivedValue, m.value);
    }
    
    TEST_F(MessageBusTestFixture, MultipleSubscribersReceiveMessage) {
        int otherValue = 0;
        bus.subscribe([&](const Message& msg) {
            auto testMsg = std::any_cast<nlohmann::json>(msg.payload);
            receivedValue = testMsg["value"].get<int>();
        });
        bus.subscribe([&](const Message& msg) {
            auto testMsg = std::any_cast<nlohmann::json>(msg.payload);
            otherValue = testMsg["value"].get<int>();
        });
    
        TestMessage m{42};
        nlohmann::json pl = { {"value", 42} }; // test payload
        Message wrapped("sensor1", "test_topic", pl);
        bus.publish(wrapped);
    
        EXPECT_EQ(receivedValue, m.value);
        EXPECT_EQ(otherValue, m.value);
    }
    