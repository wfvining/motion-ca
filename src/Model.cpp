#include "Model.hpp"

#include <numeric>   // std::accumulate
#include <algorithm> // std::for_each

/// ModelStats functions

ModelStats::ModelStats() {}
ModelStats::~ModelStats() {}

void ModelStats::PushState(double density, const NetworkSnapshot& snapshot)
{
   _network.AppendSnapshot(snapshot);
   _ca_density.push_back(density);
}

const Network& ModelStats::GetNetwork() const
{
   return _network;
}

unsigned int ModelStats::ElapsedTime() const
{
   return _network.Size();
}

bool ModelStats::IsCorrect() const
{
   if(_ca_density.size() == 0)
   {
      return false;
   }
   else if(_ca_density[0] >= 0.5)
   {
      return _ca_density.back() == 1.0;
   }
   else
   {
      return _ca_density.back() == 0.0;
   }
}

const std::vector<double>& ModelStats::GetDensityHistory() const
{
   return _ca_density;
}

/// Model functions

Model::Model(double arena_size,
             int num_agents,
             double communication_range,
             int seed,
             double initial_density) :
   _communication_range(communication_range),
   _rng(seed)
{
   std::uniform_real_distribution<double> coordinate_distribution(-arena_size/2, arena_size/2);
   std::uniform_real_distribution<double> heading_distribution(0, 2*M_PI);
   std::bernoulli_distribution state_distribution(initial_density);

   for(int i = 0; i < num_agents; i++)
   {
      Point initial_position(coordinate_distribution(_rng), coordinate_distribution(_rng));
      Heading initial_heading(heading_distribution(_rng));
      Agent a(initial_position, initial_heading, 1.0, arena_size);
      _agents.push_back(a);
      if(state_distribution(_rng))
      {
         _agent_states.push_back(1);
      }
      else
      {
         _agent_states.push_back(0);
      }
   }
   _turn_distribution = heading_distribution;
   _step_distribution = std::uniform_int_distribution<int>(1,1);
   _stats.PushState(CurrentDensity(), CurrentNetwork());
}

Model::~Model() {}

double Model::CurrentDensity() const
{
   return std::accumulate(_agent_states.begin(), _agent_states.end(), 0.0) / _agent_states.size();
}

NetworkSnapshot Model::CurrentNetwork() const
{
   NetworkSnapshot snapshot(_agents.size());
   for(int i = 0; i < _agents.size(); i++)
   {
      for(int j = i+1; j < _agents.size(); j++)
      {
         if(_agents[i].Position().Within(_communication_range, _agents[j].Position()))
         {
            snapshot.AddEdge(i, j);
         }
      }
   }
   return snapshot;
}

const ModelStats& Model::GetStats() const
{
   return _stats;
}

const std::vector<Agent>& Model::GetAgents() const
{
   return _agents;
}

void Model::SetTurnDistribution(std::function<double(std::mt19937_64&)> turn_distribution)
{
   _turn_distribution = turn_distribution;
}

void Model::SetStepDistribution(std::function<int(std::mt19937_64&)> step_distribution)
{
   _step_distribution = step_distribution;
   for(auto& agent : _agents)
   {
      agent.SetUpdateInterval(_step_distribution(_rng));
   }
}

void Model::Step(Rule* rule)
{
   for(Agent& agent : _agents)
   {
      agent.Step();
      if(agent.ShouldTurn())
      {
         agent.SetHeading(agent.GetHeading() + Heading(_turn_distribution(_rng)));
         agent.SetUpdateInterval(_step_distribution(_rng));
      }
   }

   NetworkSnapshot current_network = CurrentNetwork();
   std::vector<int> new_states(_agents.size());
   for(int a = 0; a < _agent_states.size(); a++)
   {
      auto neighbors = current_network.GetNeighbors(a);
      std::vector<int> neighbor_states;
      for(int n : neighbors)
      {
         neighbor_states.push_back(_agent_states[n]);
      }
      new_states[a] = rule(_agent_states[a], neighbor_states);
   }
   _agent_states = new_states;
   _stats.PushState(CurrentDensity(), current_network);
}
