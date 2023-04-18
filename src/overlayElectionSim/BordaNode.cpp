#include <vector>
#include "common/Utils.hpp"
#include "overlayElectionSim/Network.hpp"
#include "overlayElectionSim/BordaNode.hpp"
#include "overlayElectionSim/VoteCounter.hpp"

BordaNode::BordaNode(Network* network, uint32_t id, uint32_t candidates) : Node(network, id), m_candidates(candidates) {}
BordaNode::~BordaNode() {}

void BordaNode::init() {
	m_state = new NodeState(std::vector<uint32_t>{m_id}, std::vector<uint32_t>{0});
}

NodeState* BordaNode::vote(uint32_t round) {
	VoteCounter counter;
	std::unordered_map<uint32_t, uint32_t> timestamps;
	timestamps[m_id] = round;
	uint32_t diameter = m_network->getGraph().diameter();
	for (uint32_t neighbor : m_network->getGraph().neighbors(m_id)) {
		NodeState* vote = m_network->getNode(neighbor)->getNodeState();
		uint32_t candidates = vote->size();
		for (uint32_t i = 0; i < candidates; ++i) {
			uint32_t candidate = vote->getCandidate(i);
			uint32_t timestamp = vote->getTimestamp(i);
			if (round - timestamp <= diameter) {
				counter.vote(candidate, m_candidates - i);
				timestamps[candidate] = std::max(timestamp, timestamps[candidate]);
			}
		}
	}

	counter.endVoting();
	std::vector<uint32_t> candidates;
	std::vector<uint32_t> candidateTimestamps;
	bool selfIsCandidate = false;
	for (uint32_t i = 0; i < std::min(counter.places(), m_candidates); ++i) {
		uint32_t winner = counter.winner(i);
		candidates.push_back(winner);
		candidateTimestamps.push_back(timestamps[winner]);
		if (winner == m_id) {
			selfIsCandidate = true;
		}
	}

	if (candidates.size() < m_candidates && !selfIsCandidate) {
		candidates.push_back(m_id);
		candidateTimestamps.push_back(round);
	}

	if (candidates.size() != 0) {
		return new NodeState(std::move(candidates), std::move(candidateTimestamps));
	} else {
		return new NodeState(std::vector<uint32_t>{m_id}, std::vector<uint32_t>{round});
	}
}