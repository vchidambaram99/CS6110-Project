#ifndef _THREE_MAJORITY_NODE_HPP
#define _THREE_MAJORITY_NODE_HPP

#include "Node.hpp"

class ThreeMajorityNode : public Node {
public:
	ThreeMajorityNode(uint32_t id, uint32_t numCandidates, uint32_t diameter);
	~ThreeMajorityNode();

	void init() override;
	void vote(uint32_t round, std::vector<uint32_t>& rCandidates, std::vector<uint32_t>& rTimestamps) override;
};

#endif // _THREE_MAJORITY_NODE_HPP