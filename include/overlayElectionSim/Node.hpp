#ifndef _NODE_HPP
#define _NODE_HPP

#include <stdint.h>
#include <vector>

class Network;

class Node {
public:
	Node(uint32_t id, uint32_t numCandidates, uint32_t diameter);
	virtual ~Node();

	virtual void init() = 0;
	virtual void vote(uint32_t round, std::vector<uint32_t>& rCandidates, std::vector<uint32_t>& rTimestamps) = 0;

	void setNodeState(std::vector<uint32_t>&& candidates, std::vector<uint32_t>&& timestamps);
	void addNeighbor(Node* neighbor);
	uint32_t getSize();
	uint32_t getCandidate(uint32_t i);
	uint32_t getTimestamp(uint32_t i);

protected:
	std::vector<Node*> m_neighbors;
	uint32_t m_id;
	uint32_t m_diameter;
	uint32_t m_numCandidates;
	std::vector<uint32_t> m_candidates;
	std::vector<uint32_t> m_timestamps;
};

#endif // _NODE_HPP