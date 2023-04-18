#include <algorithm>
#include "common/Utils.hpp"
#include "overlayElectionSim/VoteCounter.hpp"

VoteCounter::VoteCounter() : m_results(), m_votes() {}
VoteCounter::~VoteCounter() {}
void VoteCounter::vote(uint32_t id, uint32_t weight) {
	auto iter = m_votes.find(id);
	if (iter == m_votes.end()) {
		m_votes[id] = weight;
	} else {
		iter->second += weight;
	}
}

void VoteCounter::endVoting() {
	m_results = std::vector<std::pair<uint32_t, uint32_t>>(m_votes.size());
	uint32_t i = 0;
	for (auto iter = m_votes.begin(); iter != m_votes.end(); ++iter) {
		m_results[i++] = std::pair<uint32_t, uint32_t>(iter->first, iter->second);
	}

	std::sort(m_results.begin(), m_results.end(), [](const std::pair<uint32_t, uint32_t>& a, const std::pair<uint32_t, uint32_t>& b) { 
		return a.second > b.second; 
	});

	uint32_t start = 0;
	for (uint32_t i = 1; i <= m_results.size(); ++i) {
		if (i == m_results.size() || m_results[i].second != m_results[start].second) {
			shuffleEqual(start, i - 1);
			start = i;
		}
	}
}

uint32_t VoteCounter::places() const {
	return m_results.size();
}

uint32_t VoteCounter::winner(uint32_t place) const {
	return m_results[place].first;
}

uint32_t VoteCounter::votes(uint32_t place) const {
	return m_results[place].second;
}
	
void VoteCounter::shuffleEqual(uint32_t idxStart, uint32_t idxEnd) {
	for (uint32_t i = idxStart; i < idxEnd; ++i) {
		std::swap(m_results[i], m_results[random(i, idxEnd)]);
	}
}