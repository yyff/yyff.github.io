/**
 *  Filename: test_func.cpp
 *   Created: 2017-11-18 20:55:48
 *      Desc: TODO (some description)
 *    Author: Yang Fan, yangfan036@gmail.com
 *   Company: 
 */

#include "func.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class FuncTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  FuncTest() {
    // You can do set-up work for each test here.
  }

  virtual ~FuncTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(FuncTest, square_sum) {
  EXPECT_EQ(5, square_sum(1, 2));
  EXPECT_EQ(10, square_sum(1, 3));
}


}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
