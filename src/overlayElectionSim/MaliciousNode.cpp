#include "overlayElectionSim/MaliciousNode.hpp"

MaliciousNode::MaliciousNode(uint32_t id, uint32_t numCandidates, uint32_t diameter) : Node(id, numCandidates, diameter) {}
MaliciousNode::~MaliciousNode() {}

void MaliciousNode::init() {
	m_candidates = std::vector<uint32_t>(m_numCandidates, 0);
	m_timestamps = std::vector<uint32_t>(m_numCandidates, 0);
	for (uint32_t i = 0; i < m_numCandidates; ++i) {
		m_candidates[i] = m_id + i;
	}
}

void MaliciousNode::vote(uint32_t round, std::vector<uint32_t>& rCandidates, std::vector<uint32_t>& rTimestamps) {
	for (uint32_t i = 0; i < m_numCandidates; ++i) {
		m_timestamps[i] = round;
	}

	rCandidates = m_candidates;
	rTimestamps = m_timestamps;
}