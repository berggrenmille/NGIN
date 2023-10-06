#include <gtest/gtest.h>
#include <NGIN/VFS/Path.hpp>

using namespace NGIN::VFS;

class PathTest : public ::testing::Test
{
protected:
    // Set-up code here.
};

/// Tests if the Path converts to a string correctly
TEST_F(PathTest, ToString)
{
    Path p("/some/path");
    EXPECT_EQ(p.ToString(), "/some/path");
}

/// Tests if the path recognizes an absolute path
TEST_F(PathTest, IsAbsolute)
{
#ifdef NGIN_PLATFORM_WINDOWS
    Path p("C:/absolute/path");
#else
    Path p("/absolute/path");
#endif
    EXPECT_TRUE(p.IsAbsolute());
}

/// Tests if the path recognizes a relative path
TEST_F(PathTest, IsRelative)
{
    Path p("relative/path");
    auto b = p.IsRelative();
    EXPECT_TRUE(b);
}

/// Tests if the path recognizes a root directory
TEST_F(PathTest, IsRoot)
{
    Path p("/");
    EXPECT_TRUE(p.IsRoot());
}

/// Tests if the path recognizes a directory
TEST_F(PathTest, IsDirectory)
{
    Path p("/some/path/");
    EXPECT_TRUE(p.IsDirectory());
}

/// Tests if the path recognizes a file
TEST_F(PathTest, IsFile)
{
    Path p("/some/file");
    EXPECT_TRUE(p.IsFile());
}

/// Tests if the path is empty
TEST_F(PathTest, IsEmpty)
{
    Path p("");
    EXPECT_TRUE(p.IsEmpty());
}

/// Tests if the path normalizes properly
TEST_F(PathTest, Normalize)
{
    Path p("/some//path/");
    EXPECT_EQ(p.ToString(), "/some/path");
}

/// Tests string conversion to native string
TEST_F(PathTest, ConvertToNative)
{
    // This will depend on platform, example for UNIX-like
    std::string original = "/some/path";
    Path p(original);
    EXPECT_EQ(p.ToString(), original);
}

/// Tests string conversion from native string
TEST_F(PathTest, ConvertFromNative)
{
    // This will depend on platform, example for UNIX-like
    Path p("/some/path");
    std::string converted = p.ToString();
    EXPECT_EQ(converted, "/some/path");
}

/// Tests edge case of multiple separators
TEST_F(PathTest, MultipleSeparators)
{
    Path p("/some////path");
    EXPECT_EQ(p.ToString(), "/some/path");
}

/// Tests edge case of trailing separator
TEST_F(PathTest, TrailingSeparator)
{
    Path p("/some/path/");
    EXPECT_TRUE(p.IsDirectory());
}

/// Tests the copy constructor
TEST_F(PathTest, CopyConstructor)
{
    Path original("/some/path");
    Path copy = original;
    EXPECT_EQ(copy.ToString(), original.ToString());
}

/// Tests the move constructor
TEST_F(PathTest, MoveConstructor)
{
    Path original("/some/path");
    Path moved = std::move(original);
    EXPECT_EQ(moved.ToString(), "/some/path");
}

/// Tests assignment operator
TEST_F(PathTest, AssignmentOperator)
{
    Path p1("/path/one");
    Path p2("/path/two");
    p1 = p2;
    EXPECT_EQ(p1.ToString(), p2.ToString());
}

/// Tests move assignment operator
TEST_F(PathTest, MoveAssignmentOperator)
{
    Path p1("/path/one");
    Path p2("/path/two");
    p1 = std::move(p2);
    EXPECT_EQ(p1.ToString(), "/path/two");
}

/// Tests the default destructor
TEST_F(PathTest, Destructor)
{
    // Not much to test for a destructor other than it's callable
    Path* p = new Path("/some/path");
    delete p;
    SUCCEED();
}

// Add more tests based on edge cases and further requirements.

