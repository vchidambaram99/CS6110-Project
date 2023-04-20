#ifndef _GRAPH_HPP
#define _GRAPH_HPP

#include <stdint.h>
#include <string>
#include <vector>

class Graph {
public:
	Graph();
	Graph(std::vector<std::vector<uint32_t>>&& adjacencyList);
	~Graph();
	bool load(const std::string& filename);
	bool store(const std::string& filename) const;

	uint32_t size() const;
	const std::vector<uint32_t>& neighbors(uint32_t node) const;

	void addNode(std::vector<uint32_t>&& neighbors);

	uint32_t diameter() const;
	void calculateDiameter();

private:
	std::vector<std::vector<uint32_t>> m_adjacencyList;
	uint32_t m_diameter;
};

#endif // _GRAPH_HPP