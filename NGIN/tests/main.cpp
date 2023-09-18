#include <gtest/gtest.h>
#include <NGIN/Logging.hpp>

class CustomTestListener : public ::testing::EmptyTestEventListener
{
public:
	virtual void OnTestStart(const ::testing::TestInfo& test_info) override
	{

	}

	virtual void OnTestEnd(const ::testing::TestInfo& test_info) override
	{

	}


};

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	// Gets hold of the event listener list.
	::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

	// Adds a listener to the end. Google Test takes the ownership.
	listeners.Append(new CustomTestListener());


	NGIN::Logging::Init();
	return RUN_ALL_TESTS();
}
