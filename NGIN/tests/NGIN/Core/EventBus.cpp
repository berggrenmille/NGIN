#include <gtest/gtest.h>
#include <NGIN/Core/EventBus.hpp>
#include <thread>
#include <chrono>

using namespace NGIN::Core;

// Define a simple event structure for testing
struct TestEvent
{
    int value;
};

struct AnotherTestEvent
{
    int value;
};

// Define a class to test member function subscriptions
class DummyClass
{
public:
    void OnTestEvent(const TestEvent& event)
    {
        receivedValue = event.value;
    }

    int receivedValue = 0;
};

// Define a static function for testing
static void StaticTestEventFunction(const TestEvent& event, int& storage)
{
    storage = event.value;
}

TEST(EventBusTest, SubscribeAndPublish)
{
    EventBus bus;
    int observedValue = 0;

    bus.Subscribe<TestEvent>([&](const TestEvent& event)
                             {
                                 observedValue = event.value;
                             });

    TestEvent event {42};
    bus.Publish(event);

    ASSERT_EQ(observedValue, 42);
}

TEST(EventBusTest, MultipleSubscribers)
{
    EventBus bus;
    int sum = 0;

    bus.Subscribe<TestEvent>([&](const TestEvent& event)
                             {
                                 sum += event.value;
                             });

    bus.Subscribe<TestEvent>([&](const TestEvent& event)
                             {
                                 sum += event.value;
                             });

    TestEvent event {10};
    bus.Publish(event);

    ASSERT_EQ(sum, 20);
}

TEST(EventBusTest, MultipleEventTypes)
{
    EventBus bus;
    int testEventSum = 0;
    int anotherTestEventSum = 0;

    bus.Subscribe<TestEvent>([&](const TestEvent& event)
                             {
                                 testEventSum += event.value;
                             });

    bus.Subscribe<AnotherTestEvent>([&](const AnotherTestEvent& event)
                                    {
                                        anotherTestEventSum += event.value;
                                    });

    TestEvent testEvent {5};
    AnotherTestEvent anotherTestEvent {10};

    bus.Publish(testEvent);
    bus.Publish(anotherTestEvent);

    ASSERT_EQ(testEventSum, 5);
    ASSERT_EQ(anotherTestEventSum, 10);
}

TEST(EventBusTest, UnsubscribedEvents)
{
    EventBus bus;
    int observedValue = 0;

    // No subscription for TestEvent
    TestEvent event {42};
    bus.Publish(event);

    ASSERT_EQ(observedValue, 0);
}

TEST(EventBusTest, StaticFunction)
{
    EventBus bus;
    int observedValue = 0;

    bus.Subscribe<TestEvent>([&](const TestEvent& event)
                             {
                                 StaticTestEventFunction(event, observedValue);
                             });

    TestEvent event {20};
    bus.Publish(event);

    ASSERT_EQ(observedValue, 20);
}

TEST(EventBusTest, MemberFunction)
{
    EventBus bus;
    DummyClass dummy;

    bus.Subscribe<TestEvent>(&dummy, &DummyClass::OnTestEvent);

    TestEvent event {15};
    bus.Publish(event);

    ASSERT_EQ(dummy.receivedValue, 15);
}

// Edge Case: Empty EventBus
TEST(EventBusTest, EmptyEventBus)
{
    EventBus bus;
    int observedValue = 0;

    // No subscription or publishing
    ASSERT_EQ(observedValue, 0);
}

// Test Queued Events
TEST(EventBusTest, QueuedEvents)
{
    EventBus bus;
    int sum = 0;

    bus.Subscribe<TestEvent>([&](const TestEvent& event)
                             {
                                 sum += event.value;
                             });

    TestEvent event {5};
    bus.Publish(event, EventMode::Queued);
    ASSERT_EQ(sum, 0); // Should still be 0 because event is queued
    bus.Publish(event, EventMode::Queued);
    
    bus.FlushEvents();
    ASSERT_EQ(sum, 10); // Should be 10 because event was flushed

}

// Add more edge cases as necessary, like handling of invalid arguments, etc.


