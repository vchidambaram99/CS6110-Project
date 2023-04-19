#include <vector>
#include "common/Utils.hpp"
#include "overlayElectionSim/Network.hpp"
#include "overlayElectionSim/BordaNode.hpp"
#include "overlayElectionSim/VoteCounter.hpp"

BordaNode::BordaNode(uint32_t id, uint32_t numCandidates, uint32_t diameter) : Node(id, numCandidates, diameter) {}
BordaNode::~BordaNode() {}

void BordaNode::init() {
	m_candidates = std::vector<uint32_t>{m_id};
	m_timestamps = std::vector<uint32_t>{0};
}

void BordaNode::vote(uint32_t round, std::vector<uint32_t>& rCandidates, std::vector<uint32_t>& rTimestamps) {
	VoteCounter counter;
	std::unordered_map<uint32_t, uint32_t> timestamps;
	timestamps[m_id] = round;
	for (Node* neighbor : m_neighbors) {
		uint32_t candidates = neighbor->getSize();
		for (uint32_t i = 0; i < candidates; ++i) {
			uint32_t candidate = neighbor->getCandidate(i);
			uint32_t timestamp = neighbor->getTimestamp(i);
			if (round - timestamp <= m_diameter) {
				counter.vote(candidate, m_numCandidates - i);
				timestamps[candidate] = std::max(timestamp, timestamps[candidate]);
			}
		}
	}

	counter.endVoting();
	std::vector<uint32_t> candidates;
	std::vector<uint32_t> candidateTimestamps;
	bool selfIsCandidate = false;
	for (uint32_t i = 0; i < std::min(counter.places(), m_numCandidates); ++i) {
		uint32_t winner = counter.winner(i);
		candidates.push_back(winner);
		candidateTimestamps.push_back(timestamps[winner]);
		if (winner == m_id) {
			selfIsCandidate = true;
		}
	}

	if (candidates.size() < m_numCandidates && !selfIsCandidate) {
		candidates.push_back(m_id);
		candidateTimestamps.push_back(round);
	}

	if (candidates.size() != 0) {
		rCandidates = std::move(candidates);
		rTimestamps = std::move(candidateTimestamps);
	} else {
		rCandidates = std::vector<uint32_t>{m_id};
		rTimestamps = std::vector<uint32_t>{round};
	}
}