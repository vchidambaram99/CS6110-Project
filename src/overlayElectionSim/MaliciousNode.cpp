#include "overlayElectionSim/MaliciousNode.hpp"

MaliciousNode::MaliciousNode(Network* network, uint32_t id, uint32_t candidates) : Node(network, id), m_candidates(candidates) {}
MaliciousNode::~MaliciousNode() {}

void MaliciousNode::init() {
	std::vector<uint32_t> candidates(m_candidates, 0);
	std::vector<uint32_t> timestamps(m_candidates, 0);
	for (uint32_t i = 0; i < m_candidates; ++i) {
		candidates[i] = m_id + i;
	}
	m_state = new NodeState(std::move(candidates), std::move(timestamps));
}

NodeState* MaliciousNode::vote(uint32_t round) {
	for (uint32_t i = 0; i < m_candidates; ++i) {
		m_state->setTimestamp(i, round);
	}

	return m_state;
}

void MaliciousNode::setNodeState(NodeState* state) {} // ignore, the malicious node's state never changes