#include <gtest/gtest.h>
#include <NGIN/Core/EventBus.hpp>

using namespace NGIN::Core;

class EventBusTest : public ::testing::Test
{
protected:
	EventBus bus;

	// Structure for a mock event.
	struct TestEvent
	{
		int data;
	};

	// This will serve as a global event handler.
	static void GlobalHandler(TestEvent &event)
	{
		event.data += 1;
	}

	// A mock class with a member event handler.
	class MockListener
	{
	public:
		int value = 0;

		void MemberHandler(TestEvent &event)
		{
			event.data += 2;
			value += 2;
		}
	};
};

TEST_F(EventBusTest, GlobalSubscription)
{
	TestEvent e{0};
	bus.Subscribe<TestEvent>(GlobalHandler);
	bus.Publish(e);
	EXPECT_EQ(e.data, 1);
}

TEST_F(EventBusTest, MemberFunctionSubscription)
{
	TestEvent e{0};
	MockListener listener;
	bus.Subscribe<TestEvent>(&listener, &MockListener::MemberHandler);
	bus.Publish(e);
	EXPECT_EQ(e.data, 2);
	EXPECT_EQ(listener.value, 2);
}

TEST_F(EventBusTest, MultipleSubscriptions)
{
	TestEvent e{0};
	MockListener listener;
	bus.Subscribe<TestEvent>(GlobalHandler);
	bus.Subscribe<TestEvent>(&listener, &MockListener::MemberHandler);
	bus.Publish(e);
	EXPECT_EQ(e.data, 3); // 1 from GlobalHandler and 2 from MemberHandler
	EXPECT_EQ(listener.value, 2);
}

// Test that checks if the event bus is able to handle events with no subscribers
TEST_F(EventBusTest, ZeroSubscriptionsEvents)
{
	TestEvent e{0};
	bus.Publish(e);
	EXPECT_EQ(e.data, 0);
}

// Test that checks if the event bus is able to handle multiple events
TEST_F(EventBusTest, MultipleSubscriptionsAndEvents)
{
	TestEvent e1{0};
	TestEvent e2{0};
	MockListener listener;
	bus.Subscribe<TestEvent>(&listener, &MockListener::MemberHandler);
	bus.Subscribe<TestEvent>(&listener, &MockListener::MemberHandler);
	bus.Publish(e1);
	bus.Publish(e2);
	EXPECT_EQ(e1.data, 4);
	EXPECT_EQ(e2.data, 4);
	EXPECT_EQ(listener.value, 8);
}

// Additional tests can be added here to cover more scenarios.
