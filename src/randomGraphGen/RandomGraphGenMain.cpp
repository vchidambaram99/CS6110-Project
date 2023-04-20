#include <iostream>
#include <algorithm>
#include "common/Graph.hpp"
#include "common/Utils.hpp"

void printUsage(char** argv) {
	printf("Usage: %s inGraph outGraph", argv[0]);
	exit(0);
}

void updatePosition(std::vector<std::pair<uint32_t, uint32_t>>& degrees, uint32_t position) {
	uint32_t l = position + 1;
	uint32_t r = degrees.size();
	while (l < r) {
		uint32_t m = (l + r) / 2;
		if (degrees[m].second <= degrees[position].second) {
			r = m;
		} else {
			l = m + 1;
		}
	}

	std::swap(degrees[position], degrees[l - 1]);
}

int main(int argc, char** argv) {
	if (argc < 3) printUsage(argv);

	Graph inGraph;
	if (!inGraph.load(argv[1])) {
		printf("Failed to load graph %s\n", argv[1]);
	} else {
		std::vector<std::vector<uint32_t>> adjList(inGraph.size());
		std::vector<std::pair<uint32_t, uint32_t>> degrees(inGraph.size());
		for (uint32_t i = 0; i < inGraph.size(); ++i) {
			degrees[i] = std::pair<uint32_t, uint32_t>(i, inGraph.neighbors(i).size());
		}
		std::sort(degrees.begin(), degrees.end(), [](std::pair<uint32_t, uint32_t> a, std::pair<uint32_t, uint32_t> b){ return  a.second > b.second; });

		int n = degrees.size() - 1;
		while (degrees[0].second > 0) {
			int neighbor = random(1, n);
			adjList[degrees[0].first].push_back(degrees[neighbor].first);
			adjList[degrees[neighbor].first].push_back(degrees[0].first);
			if (--degrees[0].second == 0) --n;
			if (--degrees[neighbor].second == 0) --n;
			updatePosition(degrees, neighbor);
			updatePosition(degrees, 0);
		}

		Graph outGraph(std::move(adjList));
		outGraph.calculateDiameter();
		printf("Randomized graph diameter was %u\n", outGraph.diameter());
		if (!outGraph.store(argv[2])) {
			printf("Failed to store graph to %s\n", argv[2]);
		}
	}

	return 0;
}