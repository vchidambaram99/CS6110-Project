#ifndef _THREE_MAJORITY_NODE_HPP
#define _THREE_MAJORITY_NODE_HPP

#include "Node.hpp"

class ThreeMajorityNode : public Node {
public:
	ThreeMajorityNode(Network* network, uint32_t id);
	~ThreeMajorityNode();

	void init() override;
	NodeState* vote(uint32_t round) override;
};

#endif // _THREE_MAJORITY_NODE_HPP