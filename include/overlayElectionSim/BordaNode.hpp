#ifndef _BORDA_NODE_HPP
#define _BORDA_NODE_HPP

#include "Node.hpp"

class BordaNode : public Node {
public:
	BordaNode(Network* network, uint32_t id, uint32_t candidates);
	~BordaNode();

	void init() override;
	NodeState* vote(uint32_t round) override;

private:
	uint32_t m_candidates;
};

#endif // _BORDA_NODE_HPP