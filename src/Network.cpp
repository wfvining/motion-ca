#include "Network.hpp"

#include <algorithm>

/// NetworkSnapshot functions

NetworkSnapshot::NetworkSnapshot(int num_vertices) :
   _num_vertices(num_vertices),
   _adjacency_list(num_vertices)
{}

NetworkSnapshot::~NetworkSnapshot() {}

void NetworkSnapshot::AddEdge(int i, int j)
{
   // reject invalid input
   if(i == j || i < 0 || j < 0 || i >= _num_vertices || j >= _num_vertices)
   {
      throw(std::out_of_range("NetworkSnapshot::AddEdge()"));
   }
   else
   {
      _adjacency_list[i].insert(j);
      _adjacency_list[j].insert(i);
   }
}

double NetworkSnapshot::Density() const
{
   double n = 0;
   for(auto& v : _adjacency_list)
   {
      n += v.size();
   }
   return n / (_num_vertices * (_num_vertices-1)); // XXX
}

std::set<int> NetworkSnapshot::GetNeighbors(int v) const
{
   if(v < 0 || v >= _num_vertices)
   {
      throw std::out_of_range("Network::GetNeighbors");
   }
   return _adjacency_list[v];
}

double NetworkSnapshot::AverageDegree() const
{
   unsigned int total_degree = 0;
   for(auto& adjacencies : _adjacency_list)
   {
      total_degree += adjacencies.size();
   }
   return total_degree / _num_vertices;
}

std::vector<unsigned int> NetworkSnapshot::DegreeDistribution() const
{
   std::vector<unsigned int> degree_distribution(_num_vertices);
   std::fill(degree_distribution.begin(), degree_distribution.end(), 0);
   for(auto& al : _adjacency_list)
   {
      unsigned int degree = al.size();
      degree_distribution[degree] += 1;
   }
   return degree_distribution;
}

std::vector<double> NetworkSnapshot::NormalizedDegreeDistribution() const
{
   std::vector<unsigned int> degree_distribution = DegreeDistribution();
   std::vector<double> normalized_distribution(degree_distribution.size());
   std::transform(degree_distribution.begin(), degree_distribution.end(),
                  normalized_distribution.begin(),
                  [this](unsigned int d) { return (double)d/(double)_num_vertices; });
   return normalized_distribution;
}

int NetworkSnapshot::EdgeCount() const
{
   int n = 0;
   for(auto& al : _adjacency_list)
   {
      n += al.size();
   }
   return n / 2;
}

bool operator== (const NetworkSnapshot& s, const NetworkSnapshot& g)
{
   return s._adjacency_list == g._adjacency_list;
}

/// Network functions

Network::Network() {}

Network::~Network() {}

void Network::AppendSnapshot(std::shared_ptr<NetworkSnapshot> snapshot)
{
   _snapshots.push_back(snapshot);
}

std::shared_ptr<NetworkSnapshot> Network::GetSnapshot(unsigned int t) const
{
   if(t > _snapshots.size())
   {
      throw(std::out_of_range("Network::GetSnapshot()"));
   }
   else
   {
      return _snapshots[t];
   }
}

unsigned int Network::Size() const
{
   return _snapshots.size();
}
