#include <arpa/inet.h> // for big-endian to host conversions
#include <deque>
#include <fstream>
#include <utility>
#include "common/Graph.hpp"

uint32_t read32(std::ifstream& file) {
	uint32_t value;
	file.read(reinterpret_cast<char*>(&value), sizeof(uint32_t));
	return ntohl(value);
}

void write32(std::ofstream& file, uint32_t value) {
	value = htonl(value);
	file.write(reinterpret_cast<const char*>(&value), sizeof(uint32_t));
}

Graph::Graph() : m_adjacencyList(), m_diameter(0) {}

Graph::~Graph() {}

bool Graph::load(const std::string& filename) {
	bool status = false;
	std::ifstream file(filename);
	uint32_t numNodes = read32(file);
	m_adjacencyList.reserve(numNodes);
	for (uint32_t i = 0; i < numNodes && !file.fail(); ++i) {
		uint32_t numNeighbors = read32(file);
		std::vector<uint32_t> neighbors;
		neighbors.reserve(numNeighbors);
		for (uint32_t j = 0; j < numNeighbors && !file.fail(); ++j) {
			neighbors.push_back(read32(file));
		}
		m_adjacencyList.push_back(neighbors);
	}

	bool success = !file.fail();
	if (success) {
		m_diameter = read32(file);
		if (file.fail() || m_diameter == 0) { // calculate and store the diameter if it isn't present in the file
			calculateDiameter();
			store(filename);
		}
	}
	return !file.fail();
}

bool Graph::store(const std::string& filename) const {
	std::ofstream file(filename);
	write32(file, m_adjacencyList.size());
	for (uint32_t i = 0; i < m_adjacencyList.size() && !file.fail(); ++i) {
		write32(file, m_adjacencyList[i].size());
		for (uint32_t j = 0; j < m_adjacencyList[i].size() && !file.fail(); ++j) {
			write32(file, m_adjacencyList[i][j]);
		}
	}
	write32(file, m_diameter);
	return !file.fail();
}

uint32_t Graph::size() const {
	return m_adjacencyList.size();
}

const std::vector<uint32_t>& Graph::neighbors(uint32_t node) const {
	return m_adjacencyList[node];
}

void Graph::addNode(std::vector<uint32_t>&& neighbors) {
	m_adjacencyList.push_back(std::move(neighbors));
	for (uint32_t i = 0; i < m_adjacencyList.back().size(); ++i) {
		m_adjacencyList[m_adjacencyList.back()[i]].push_back(m_adjacencyList.size() - 1);
	}
}

uint32_t Graph::diameter() const {
	return m_diameter;
}

void Graph::calculateDiameter() {
	printf("Computing graph diameter, may take a few minutes...\n");
	m_diameter = 0;
	uint32_t n = m_adjacencyList.size();
	for (uint32_t i = 0; i < n; ++i) {
		std::vector<bool> visited(n, false);
		std::deque<std::pair<uint32_t, uint32_t>> q;
		uint32_t distance = 0;
		q.push_back(std::pair<uint32_t, uint32_t>(i, 0));
		visited[i] = true;
		while (!q.empty()) {
			distance = q.front().second;
			for (uint32_t j : m_adjacencyList[q.front().first]) {
				if (!visited[j]) {
					q.push_back(std::pair<uint32_t, uint32_t>(j, distance + 1));
			        visited[j] = true;
				}
			}
			q.pop_front();
		}
		m_diameter = std::max(m_diameter, distance);
	}
}