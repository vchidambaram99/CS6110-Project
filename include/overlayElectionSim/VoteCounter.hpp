#ifndef _VOTE_COUNTER_HPP
#define _VOTE_COUNTER_HPP

#include <stdint.h>
#include <unordered_map>
#include <utility>
#include <vector>

class VoteCounter {
public:
	VoteCounter();
	~VoteCounter();

	void vote(uint32_t id, uint32_t weight);
	void endVoting();
	uint32_t places() const;
	uint32_t winner(uint32_t place) const;
	uint32_t votes(uint32_t place) const;

private:
	void shuffleEqual(uint32_t idxStart, uint32_t idxEnd);
	std::vector<std::pair<uint32_t, uint32_t>> m_results;
	std::unordered_map<uint32_t, uint32_t> m_votes;
};

#endif // _VOTE_COUNTER_HPP