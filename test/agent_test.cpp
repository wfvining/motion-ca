#include <gtest/gtest.h>

#include "Agent.hpp"
#include "Point.hpp"

class AgentTest : public ::testing::Test
{
public:
   Agent agent;
   Agent agent_pos_x;
   Agent agent_neg_x;
   Agent agent_pos_y;
   Agent agent_neg_y;

   Heading agent_pos_x_heading;
   Heading agent_pos_y_heading;
   Heading agent_neg_x_heading;
   Heading agent_neg_y_heading;

   AgentTest() :
      agent(Point(0,0), Heading(0), 1, 10),
      agent_pos_x(Point(0,0), Heading(0), 1, 10),
      agent_neg_x(Point(0,0), Heading(M_PI), 1, 10),
      agent_pos_y(Point(0,0), Heading(M_PI_2), 1, 10),
      agent_neg_y(Point(0,0), Heading(-M_PI_2), 1, 10)
      {
         agent_pos_x_heading = agent_pos_x.GetHeading();
         agent_pos_y_heading = agent_pos_y.GetHeading();
         agent_neg_x_heading = agent_neg_x.GetHeading();
         agent_neg_y_heading = agent_neg_y.GetHeading();
      }
};

TEST_F(AgentTest, getPosition)
{
   Point p(0,0);
   EXPECT_EQ(p, agent.Position());
}

TEST_F(AgentTest, stepNoBounce)
{
   agent_pos_x.Step();
   agent_neg_x.Step();
   agent_pos_y.Step();
   agent_neg_y.Step();

   EXPECT_TRUE(agent_pos_x.Position().Within(0.0000001, Point(1,0))) << agent_pos_x.Position();
   EXPECT_TRUE(agent_neg_x.Position().Within(0.0000001, Point(-1,0)));
   EXPECT_TRUE(agent_pos_y.Position().Within(0.0000001, Point(0,1)));
   EXPECT_TRUE(agent_neg_y.Position().Within(0.0000001, Point(0,-1)));
}

TEST_F(AgentTest, bouncePerpendicularPosition)
{
   for(int i=0; i < 5; i++)
   {
      agent_pos_x.Step();
      agent_neg_x.Step();
      agent_pos_y.Step();
      agent_neg_y.Step();
   }
   EXPECT_TRUE(agent_pos_x.Position().Within(0.0000001, Point(5,0)));
   EXPECT_TRUE(agent_neg_x.Position().Within(0.0000001, Point(-5,0)));
   EXPECT_TRUE(agent_pos_y.Position().Within(0.0000001, Point(0,5)));
   EXPECT_TRUE(agent_neg_y.Position().Within(0.0000001, Point(0,-5)));

   agent_pos_x.Step();
   agent_neg_x.Step();
   agent_pos_y.Step();
   agent_neg_y.Step();
   EXPECT_TRUE(agent_pos_x.Position().Within(0.0000001, Point(4,0))) << "Agent is at: " << agent_pos_x.Position();
   EXPECT_TRUE(agent_neg_x.Position().Within(0.0000001, Point(-4,0))) << "Agent is at: " << agent_neg_x.Position();
   EXPECT_TRUE(agent_pos_y.Position().Within(0.0000001, Point(0,4))) << "Agent is at: " << agent_pos_y.Position();
   EXPECT_TRUE(agent_neg_y.Position().Within(0.0000001, Point(0,-4))) << "Agent is at: " << agent_neg_y.Position();

   Agent a(Point(5,0), Heading(0), 2, 10);
   a.Step();
   EXPECT_TRUE(a.Position().Within(0.0000001, Point(3, 0)));

   Agent a_slow(Point(5,0), Heading(0), 0.5, 10);
   a_slow.Step();
   EXPECT_TRUE(a_slow.Position().Within(0.0000001, Point(4.5, 0)));

}

TEST_F(AgentTest, bouncePerpendicularHeading)
{
   for(int i=0; i < 5; i++)
   {
      agent_pos_x.Step();
      agent_neg_x.Step();
      agent_pos_y.Step();
      agent_neg_y.Step();
   }
   agent_pos_x.Step();
   agent_neg_x.Step();
   agent_pos_y.Step();
   agent_neg_y.Step();

   EXPECT_EQ(agent_pos_x.GetHeading(), agent_pos_x_heading + Heading(M_PI));
   EXPECT_EQ(agent_neg_x.GetHeading(), agent_neg_x_heading + Heading(M_PI));
   EXPECT_EQ(agent_pos_y.GetHeading(), agent_pos_y_heading + Heading(M_PI));
   EXPECT_EQ(agent_neg_y.GetHeading(), agent_neg_y_heading + Heading(M_PI));
}