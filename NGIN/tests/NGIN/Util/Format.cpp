#include <gtest/gtest.h>
#include <NGIN/Util/Format.hpp>

using namespace NGIN::Util;

class FormatTest : public ::testing::Test
{
protected:
    // Sample string literals for testing.
    static constexpr const char *formatStringLiteral = "Hello, {}!";
    static constexpr const char *expectedFormattedString = "Hello, World!";
};

TEST_F(FormatTest, FormatLiteralString)
{
    std::string result = Format(formatStringLiteral, "World");
    EXPECT_EQ(result, expectedFormattedString);
}

TEST_F(FormatTest, RuntimeFormatString)
{
    std::string_view formatStringView = formatStringLiteral;
    std::string result = RuntimeFormat(formatStringView, "World");
    EXPECT_EQ(result, expectedFormattedString);
}

TEST_F(FormatTest, FormatMultipleArgs)
{

    std::string result = Format("{} + {} = {}", 5, 7, 5 + 7);
    EXPECT_EQ(result, "5 + 7 = 12");
}

TEST_F(FormatTest, RuntimeFormatMultipleArgs)
{
    std::string_view formatStrView = "{} + {} = {}";
    int a = 5, b = 7;
    std::string result = RuntimeFormat(formatStrView, a, b, a + b);
    EXPECT_EQ(result, "5 + 7 = 12");
}
TEST_F(FormatTest, FormatWithSpecialCharacters)
{
    std::string result = Format("{}\n\t\r%$#@", "Test");
    EXPECT_EQ(result, "Test\n\t\r%$#@");
}

TEST_F(FormatTest, FormatLargeString)
{
    std::string largeStr(1'000'000, 'A');
    std::string result = Format("{}", largeStr);
    EXPECT_EQ(result, largeStr);
}

TEST_F(FormatTest, FormatVariousDataTypes)
{
    std::string result = Format("Int: {}, Float: {:.2f}, Bool: {}", 42, 3.14159, true);
    EXPECT_EQ(result, "Int: 42, Float: 3.14, Bool: true");
}

TEST_F(FormatTest, RuntimeFormatMissingArguments)
{
    try
    {
        std::string_view formatStrView = "Hello, {} and {}!";
        // Missing one argument
        std::string result = RuntimeFormat(formatStrView, "Alice");
        FAIL();
    }
    catch (const std::exception &)
    {
        SUCCEED();
    }
}

TEST_F(FormatTest, RuntimeFormatExtraArguments)
{
    std::string_view formatStrView = "Hello, {}!";
    // Extra argument provided
    std::string result = RuntimeFormat(formatStrView, "Alice", "Bob");
    EXPECT_EQ(result, "Hello, Alice!"); // The extra argument might be ignored or could throw an error
}

TEST_F(FormatTest, FormatNestedPlaceholders)
{
    std::string result = Format("{{}}", "Test");
    EXPECT_EQ(result, "{}"); // Assuming double braces escape a single brace
}

TEST_F(FormatTest, FormatReorderedArguments)
{
    std::string result = Format("{1} before {0}", "B", "A");
    EXPECT_EQ(result, "A before B");
}

TEST_F(FormatTest, FormatNegativeValues)
{
    std::string result = Format("{}", -42);
    EXPECT_EQ(result, "-42");
}

TEST_F(FormatTest, FormatPrecisionAndWidth)
{
    std::string result = Format("{:.5} and {:5}", 3.1415926535, 42);
    EXPECT_EQ(result, "3.1416 and    42"); // Assuming right alignment for width
}

TEST_F(FormatTest, FormatLargeNumberOfArguments)
{
    // Using 50 placeholders and 50 integers
    constexpr const char *formatStr = "{} {} {} {} {} {} {} {} {} {} "
                                      "{} {} {} {} {} {} {} {} {} {} "
                                      "{} {} {} {} {} {} {} {} {} {} "
                                      "{} {} {} {} {} {} {} {} {} {} "
                                      "{} {} {} {} {} {} {} {} {} {}";

    std::string result = Format(formatStr,
                                1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                                31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                                41, 42, 43, 44, 45, 46, 47, 48, 49, 50);

    // Expected output: "1 2 3 4 ... 50" (with spaces)
    std::string expected = "1 2 3 4 5 6 7 8 9 10 "
                           "11 12 13 14 15 16 17 18 19 20 "
                           "21 22 23 24 25 26 27 28 29 30 "
                           "31 32 33 34 35 36 37 38 39 40 "
                           "41 42 43 44 45 46 47 48 49 50";

    EXPECT_EQ(result, expected);
}
