#include <gmock/gmock.h>

#include "Model.hpp"

TEST(ModelTest, expectedDensityIsCorrect)
{
   double sum = 0;
   for(int i = 0; i < 100; i++)
   {
      Model m(100, 256, 5.0, 1337+i, 0.5);
      sum += m.CurrentDensity();
   }
   EXPECT_THAT(sum/100.0, ::testing::DoubleNear(0.5, 0.01));

   sum = 0;
   for(int i = 0; i < 100; i++)
   {
      Model m(100, 256, 5.0, 1337+i, 0.25);
      sum += m.CurrentDensity();
   }
   EXPECT_THAT(sum/100.0, ::testing::DoubleNear(0.25, 0.01));

   sum = 0;
   for(int i = 0; i < 100; i++)
   {
      Model m(100, 256, 5.0, 1337+i, 0.75);
      sum += m.CurrentDensity();
   }
   EXPECT_THAT(sum/100.0, ::testing::DoubleNear(0.75, 0.01));
}

TEST(ModelTest, densityExtremes)
{
   Model m0(100, 256, 5.0, 12345, 0.0);
   Model m1(100, 256, 5.0, 12345, 1.0);

   EXPECT_EQ(m0.CurrentDensity(), 0.0);
   EXPECT_EQ(m1.CurrentDensity(), 1.0);
}

TEST(ModelTest, differentSeedDifferentDensity)
{
   Model m1(100, 128, 5.0, 1337, 0.5);
   Model m2(100, 128, 5.0, 1338, 0.5);

   EXPECT_NE(m1.CurrentDensity(), m2.CurrentDensity());
}

TEST(ModelTest, extremelySmallArena)
{
   Model m1(0.25, 32, 5.0, 123, 0.5);
}

TEST(ModelTest, networkChanges)
{
   Model m(50, 128, 5.0, 1337, 0.5);
   for(int i = 0; i < 25; i++)
   {
      m.Step(&identity_rule);
   }
   const ModelStats& stats = m.GetStats();
   const Network& network = stats.GetNetwork();
   EXPECT_FALSE(network.GetSnapshot(0) == network.GetSnapshot(24));
}

TEST(ModelTest, identityRuleUpdate)
{
   Model m(10, 25, 1.0, 1234, 0.5);
   double initial_density = m.CurrentDensity();
   for(int i = 0; i < 100; i++)
   {
      m.Step(&identity_rule);
      EXPECT_EQ(initial_density, m.CurrentDensity());
   }
}

TEST(ModelTest, majorityRuleUnderFullyConnectedGraph)
{
   Model m(10, 50, 20, 1234, 0.7);
   double initial_density = m.CurrentDensity();
   m.Step(majority_rule);
   EXPECT_EQ(1.0, m.CurrentDensity());
}

TEST(ModelTest, alwaysOneRule)
{
   Model m(10, 25, 1.0, 1234, 0.5);
   m.Step(&always_one);
   EXPECT_EQ(m.CurrentDensity(), 1.0);
}

TEST(ModelTest, alwaysZeroRule)
{
   Model m(10, 25, 1.0, 1234, 0.5);
   m.Step(&always_zero);
   EXPECT_EQ(m.CurrentDensity(), 0.0);
}

class ConstantHeading : public MovementRule
{
   double h;
   int i = 0;
public:
   ConstantHeading(double h) : h(h) {}
   ~ConstantHeading() {}
   virtual Heading Turn(const Point&, const Heading& heading, std::mt19937_64&) override
      {
         if(i++ >= 1) {
            return Heading(h);
         }
         return heading;
      }
   virtual std::shared_ptr<MovementRule> Clone() const override
      {
         return std::make_shared<ConstantHeading>(*this);
      }
};

TEST(ModelTest, agentsUpddate)
{
   Model m(1000, 1, 1.0, 1234, 0.5);
   m.SetMovementRule(ConstantHeading(M_PI));
   Heading initial_heading = m.GetAgents()[0].GetHeading();
   m.Step(majority_rule);
   EXPECT_EQ(initial_heading, m.GetAgents()[0].GetHeading());
   m.Step(majority_rule);
   EXPECT_EQ(Heading(M_PI), m.GetAgents()[0].GetHeading());
}

TEST(ModelTest, agentSpeedZero)
{
   Model m(100, 10, 1.0, 1234, 0.5, 0.0);
   auto agents = m.GetAgents();
   m.Step(identity_rule);
   for(int i = 0; i < agents.size(); i++)
   {
      ASSERT_EQ(agents[i].Position(), m.GetAgents()[i].Position());
   }
}

TEST(ModelTest, agentSpeedOneHalf)
{
   Model m(100, 10, 1.0, 1234, 0.5, 0.5);
   auto agents = m.GetAgents();
   m.Step(identity_rule);
   for(int i = 0; i < agents.size(); i++)
   {
      ASSERT_THAT(agents[i].Position().Distance(m.GetAgents()[i].Position()),
                  ::testing::DoubleNear(0.5, 0.00000000000001));
   }
}

TEST(ModelTest, agentSpeedHigh)
{
   Model m(100, 10, 1.0, 1234, 0.5, 200);
   for(int i = 0; i < 1000; i++)
   {
      m.Step(identity_rule);
      auto agents = m.GetAgents();
      for(auto& agent : agents)
      {
         ASSERT_TRUE(agent.Position().GetX() <= 50.0 && agent.Position().GetX() >= -50.0);
         ASSERT_TRUE(agent.Position().GetY() <= 50.0 && agent.Position().GetY() >= -50.0);
      }
   }
}