#ifndef _MALICIOUS_NODE_HPP
#define _MALICIOUS_NODE_HPP

#include "Node.hpp"

class MaliciousNode : public Node {
public:
	MaliciousNode(uint32_t id, uint32_t numCandidates, uint32_t diameter);
	virtual ~MaliciousNode();

	void init() override;
	void vote(uint32_t round, std::vector<uint32_t>& rCandidates, std::vector<uint32_t>& rTimestamps) override;
};

#endif // _MALICIOUS_NODE_HPP