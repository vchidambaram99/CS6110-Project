#ifndef _NODE_HPP
#define _NODE_HPP

#include <stdint.h>
#include <vector>

class Network;

class NodeState
{
public:
	NodeState(std::vector<uint32_t>&& candidates, std::vector<uint32_t>&& timestamps);
	~NodeState();

	uint32_t size() const;
	uint32_t getCandidate(uint32_t i) const;
	uint32_t getTimestamp(uint32_t i) const;

	void setCandidate(uint32_t i, uint32_t candidate);
	void setTimestamp(uint32_t i, uint32_t timestamp);

private:
	std::vector<uint32_t> m_candidates;
	std::vector<uint32_t> m_timestamps;
};

class Node {
public:
	Node(Network* network, uint32_t id);
	virtual ~Node();

	virtual void init() = 0;

	virtual NodeState* vote(uint32_t round) = 0;
	virtual NodeState* getNodeState();
	virtual void setNodeState(NodeState* state);

protected:
	Network* m_network;
	uint32_t m_id;
	NodeState* m_state;
};

#endif // _NODE_HPP