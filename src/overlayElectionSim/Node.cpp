#include "overlayElectionSim/Node.hpp"

Node::Node(uint32_t id, uint32_t numCandidates, uint32_t diameter)
  :	m_neighbors(),
	m_id(id), 
	m_numCandidates(numCandidates), 
	m_diameter(diameter),
	m_candidates(),
	m_timestamps() {}

Node::~Node() {}

void Node::setNodeState(std::vector<uint32_t>&& candidates, std::vector<uint32_t>&& timestamps) {
	m_candidates = std::move(candidates);
	m_timestamps = std::move(timestamps);
}

void Node::addNeighbor(Node* neighbor) {
	m_neighbors.push_back(neighbor);
}

uint32_t Node::getSize() {
	return m_candidates.size();
}

uint32_t Node::getCandidate(uint32_t i) {
	return m_candidates[i];
}

uint32_t Node::getTimestamp(uint32_t i) {
	return m_timestamps[i];
}