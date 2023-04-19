#ifndef _NETWORK_HPP
#define _NETWORK_HPP

#include <functional>
#include <map>
#include <utility>

#include "common/Graph.hpp"
#include "Node.hpp"

struct Statistics {
	Statistics();
	
	uint32_t m_simulationStep;
	uint32_t m_numLeaders;
	uint32_t m_leaderId;
	float m_agreementRatio;
	float m_leaderChangeRatio;
};

class Network {
public:
	typedef std::function<Node*(uint32_t, uint32_t, uint32_t)> NodeFactory;
	Network(const std::string& filename, NodeFactory f, uint32_t candidates);
	~Network();

	const Graph& getGraph();

	uint32_t addMaliciousNode(uint32_t edges, uint32_t candidates);

	const Statistics& getStatistics();
	const Statistics& stepSimulation();

private:	
	Statistics m_stats;
	Graph m_graph;
	std::vector<Node*> m_nodes;
};

#endif // _NETWORK_HPP