#ifndef _MALICIOUS_NODE_HPP
#define _MALICIOUS_NODE_HPP

#include "Node.hpp"

class MaliciousNode : public Node {
public:
	MaliciousNode(Network* network, uint32_t id, uint32_t candidates);
	virtual ~MaliciousNode();

	void init() override;

	NodeState* vote(uint32_t round) override;
	void setNodeState(NodeState* state) override;

private:
	uint32_t m_candidates;
};

#endif // _MALICIOUS_NODE_HPP