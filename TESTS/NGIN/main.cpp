#include <gtest/gtest.h>
#include <NGIN/Logger.h>

class CustomTestListener : public ::testing::EmptyTestEventListener
{
public:
	virtual void OnTestStart(const ::testing::TestInfo& test_info) override
	{
		NGIN::Logger::setupLoggingForTests();
	}

	virtual void OnTestEnd(const ::testing::TestInfo& test_info) override
	{
		NGIN::Logger::cleanupLoggingForTests();
	}


};

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	// Gets hold of the event listener list.
	::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

	// Adds a listener to the end. Google Test takes the ownership.
	listeners.Append(new CustomTestListener());

	std::cout << "Red ERR outputs are ok. Usually it's an expected assert." << std::endl;

	return RUN_ALL_TESTS();
}
