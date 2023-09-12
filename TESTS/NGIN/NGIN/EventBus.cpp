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

// Additional tests can be added here to cover more scenarios.
