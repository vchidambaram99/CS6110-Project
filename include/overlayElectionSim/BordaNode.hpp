#ifndef _BORDA_NODE_HPP
#define _BORDA_NODE_HPP

#include "Node.hpp"

class BordaNode : public Node {
public:
	BordaNode(uint32_t id, uint32_t numCandidates, uint32_t diameter);
	~BordaNode();

	void init() override;
	void vote(uint32_t round, std::vector<uint32_t>& rCandidates, std::vector<uint32_t>& rTimestamps) override;
};

#endif // _BORDA_NODE_HPP