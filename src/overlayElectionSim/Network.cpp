#include <set>
#include "common/Utils.hpp"
#include "overlayElectionSim/MaliciousNode.hpp"
#include "overlayElectionSim/Network.hpp"
#include "overlayElectionSim/VoteCounter.hpp"

Statistics::Statistics() : m_simulationStep(0), m_leaderId(0), m_numLeaders(0), m_agreementRatio(0.0f), m_leaderChangeRatio(0.0f) {}

Network::Network(const std::string& filename, NodeFactory f) : m_graph(), m_nodes(){
	if (m_graph.load(filename)) {
		m_nodes = std::vector<Node*>(m_graph.size());
		for (uint32_t i = 0; i < m_nodes.size(); ++i) {
			m_nodes[i] = f(this, i);
			m_nodes[i]->init();
		}
	} else {
		printf("Failed to load graph %s", filename.c_str());
		exit(0);
	}
}

Network::~Network() {
	for (Node* node : m_nodes) {
		delete node;
	}
}

const Statistics& Network::getStatistics() {
	return m_stats;
}

const Statistics& Network::stepSimulation() {
	++m_stats.m_simulationStep;
	uint32_t leaderChanges = 0;
	std::vector<NodeState*> newStates(m_nodes.size());
	for (uint32_t i = 0; i < m_nodes.size(); ++i) {
		newStates[i] = m_nodes[i]->vote(m_stats.m_simulationStep);
	}

	for (uint32_t i = 0; i < m_nodes.size(); ++i) {
		if (m_nodes[i]->getNodeState()->getCandidate(0) != newStates[i]->getCandidate(0)) {
			leaderChanges++;
		}
		m_nodes[i]->setNodeState(newStates[i]);
	}

	std::vector<uint32_t> votes(m_nodes.size(), 0);
	uint32_t winner = 0;
	uint32_t numLeaders = 0;
	for (uint32_t i = 0; i < m_nodes.size(); ++i) {
		uint32_t candidate = newStates[i]->getCandidate(0);
		if (votes[candidate] == 0) ++numLeaders;
		if (++votes[candidate] > votes[winner]) {
			winner = candidate;
		}
	}
	m_stats.m_leaderId = winner;
	m_stats.m_numLeaders = numLeaders;
	m_stats.m_agreementRatio = static_cast<float>(votes[winner]) / m_nodes.size();
	m_stats.m_leaderChangeRatio = static_cast<float>(leaderChanges) / m_nodes.size();

	return m_stats;
}

Node* Network::getNode(uint32_t id) {
	return m_nodes[id];
}

const Graph& Network::getGraph() {
	return m_graph;
}

uint32_t Network::addMaliciousNode(uint32_t edges, uint32_t candidates)
{
	std::set<uint32_t> neighborSet;
	while (neighborSet.size() < edges) {
		neighborSet.insert(random(0, m_graph.size() - 1));
	}

	std::vector<uint32_t> neighbors(edges, 0);
	uint32_t i = 0;
	for (auto iter = neighborSet.begin(); iter != neighborSet.end(); ++iter, ++i) {
		neighbors[i] = *iter;
	}

	uint32_t maliciousId = m_graph.size();
	m_nodes.push_back(new MaliciousNode(this, maliciousId, candidates));
	m_nodes.back()->init();
	m_graph.addNode(std::move(neighbors));
	return maliciousId;
}