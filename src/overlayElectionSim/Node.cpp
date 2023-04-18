#include "overlayElectionSim/Node.hpp"

NodeState::NodeState(std::vector<uint32_t>&& candidates, std::vector<uint32_t>&& timestamps) :
	m_candidates(std::move(candidates)),
	m_timestamps(std::move(timestamps)) {}

NodeState::~NodeState() {}

uint32_t NodeState::size() const {
	return m_candidates.size();
}

uint32_t NodeState::getCandidate(uint32_t i) const {
	return m_candidates[i];
}

uint32_t NodeState::getTimestamp(uint32_t i) const {
	return m_timestamps[i];
}

void NodeState::setCandidate(uint32_t i, uint32_t candidate) {
	m_candidates[i] = candidate;
}

void NodeState::setTimestamp(uint32_t i, uint32_t timestamp) {
	m_timestamps[i] = timestamp;
}

Node::Node(Network* network, uint32_t id) : m_network(network), m_id(id), m_state(nullptr) {}
Node::~Node() {
	if (m_state != nullptr) {
		delete m_state;
	}
}

NodeState* Node::getNodeState() {
	return m_state;
}

void Node::setNodeState(NodeState* state) {
	if (m_state != nullptr) {
		delete m_state;
	}
	m_state = state;
}