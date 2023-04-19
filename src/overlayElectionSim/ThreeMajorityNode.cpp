#include <vector>
#include "common/Utils.hpp"
#include "overlayElectionSim/Network.hpp"
#include "overlayElectionSim/ThreeMajorityNode.hpp"
#include "overlayElectionSim/VoteCounter.hpp"

ThreeMajorityNode::ThreeMajorityNode(uint32_t id, uint32_t numCandidates, uint32_t diameter) : Node(id, numCandidates, diameter) {}
ThreeMajorityNode::~ThreeMajorityNode() {}

void ThreeMajorityNode::init() {
	m_candidates = std::vector<uint32_t>{m_id};
	m_timestamps = std::vector<uint32_t>{0};
}

void ThreeMajorityNode::vote(uint32_t round, std::vector<uint32_t>& rCandidates, std::vector<uint32_t>& rTimestamps) {
	std::vector<Node*> validVotes;
	std::unordered_map<uint32_t, uint32_t> timestamps;
	timestamps[m_id] = round;
	for (Node* neighbor : m_neighbors) {
		uint32_t candidate = neighbor->getCandidate(0);
		uint32_t timestamp = neighbor->getTimestamp(0);
		if (round - timestamp <= m_diameter)
		{
			validVotes.push_back(neighbor);
			timestamps[candidate] = std::max(timestamp, timestamps[candidate]);
		}
	}

	if (validVotes.size() > 0) {
		VoteCounter counter;
		for (uint32_t i = 0; i < 3; ++i) {
			counter.vote(validVotes[random(0, validVotes.size() - 1)]->getCandidate(0), 1);
		}
		counter.endVoting();

		uint32_t leader = counter.winner(0);
		uint32_t leaderTimestamp = timestamps[leader];

		rCandidates = std::vector<uint32_t>{leader};
		rTimestamps = std::vector<uint32_t>{leaderTimestamp};
	} else {
		rCandidates = std::vector<uint32_t>{m_id};
		rTimestamps = std::vector<uint32_t>{round};
	}
}