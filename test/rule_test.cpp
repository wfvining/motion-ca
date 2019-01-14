#include <gtest/gtest.h>

#include "Rule.hpp"

class RuleTest : public ::testing::Test
{
public:
   MajorityRule     majority_rule;
   std::vector<int> half_ones = {0,1,0,1};
   std::vector<int> all_ones  = {1,1,1,1};
   std::vector<int> all_zeros = {0,0,0,0};
   std::vector<int> slim_majority_ones =  {0,0,1,1,1,1};
   std::vector<int> slim_majority_zeros = {0,0,0,0,1,1};
   RuleTest() {}
   ~RuleTest() {}
};

TEST_F(RuleTest, slimMajorityWithSelf)
{
   EXPECT_EQ(1, majority_rule.Apply(1, half_ones));
   EXPECT_EQ(0, majority_rule.Apply(0, half_ones));
}

TEST_F(RuleTest, slimMajorityAgainstSelf)
{
   EXPECT_EQ(1, majority_rule.Apply(0, slim_majority_ones));
   EXPECT_EQ(0, majority_rule.Apply(1, slim_majority_zeros));
}

TEST_F(RuleTest, absoluteMajority)
{
   EXPECT_EQ(1, majority_rule.Apply(1, all_ones ));
   EXPECT_EQ(1, majority_rule.Apply(0, all_ones ));
   EXPECT_EQ(0, majority_rule.Apply(0, all_zeros));
   EXPECT_EQ(0, majority_rule.Apply(1, all_zeros));
}
