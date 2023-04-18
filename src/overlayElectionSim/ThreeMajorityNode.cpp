#include <vector>
#include "common/Utils.hpp"
#include "overlayElectionSim/Network.hpp"
#include "overlayElectionSim/ThreeMajorityNode.hpp"
#include "overlayElectionSim/VoteCounter.hpp"

ThreeMajorityNode::ThreeMajorityNode(Network* network, uint32_t id) : Node(network, id) {}
ThreeMajorityNode::~ThreeMajorityNode() {}

void ThreeMajorityNode::init() {
	m_state = new NodeState(std::vector<uint32_t>{m_id}, std::vector<uint32_t>{0});
}

NodeState* ThreeMajorityNode::vote(uint32_t round) {
	std::vector<NodeState*> validVotes;
	std::unordered_map<uint32_t, uint32_t> timestamps;
	timestamps[m_id] = round;
	uint32_t diameter = m_network->getGraph().diameter();
	for (uint32_t neighbor : m_network->getGraph().neighbors(m_id)) {
		NodeState* vote = m_network->getNode(neighbor)->getNodeState();
		uint32_t candidate = vote->getCandidate(0);
		uint32_t timestamp = vote->getTimestamp(0);
		if (round - timestamp <= diameter)
		{
			validVotes.push_back(vote);
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

		return new NodeState(std::vector<uint32_t>{leader}, std::vector<uint32_t>{leaderTimestamp});
	} else {
		return new NodeState(std::vector<uint32_t>{m_id}, std::vector<uint32_t>{round});
	}
}