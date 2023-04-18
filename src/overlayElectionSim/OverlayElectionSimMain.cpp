#include <getopt.h>
#include <iostream>

#include "common/Utils.hpp"
#include "overlayElectionSim/BordaNode.hpp"
#include "overlayElectionSim/ThreeMajorityNode.hpp"
#include "overlayElectionSim/Network.hpp"

void printUsage(char** argv) {
	printf("Usage: %s (-g | --graph <filename>) (-n | --node-type <node type>) [-m | --malicious <attack edges>] [-r | --rounds <rounds>] [-s | --seed <random seed>] [-v | --verbose] [-h|--help]\n", argv[0]);
	exit(0);
}

int main(int argc, char** argv) {
	static struct option long_options[] = {
		{"help",      no_argument,       0, 'h'},
		{"graph",     required_argument, 0, 'g'},
		{"node-type", required_argument, 0, 'n'},
		{"malicious", required_argument, 0, 'm'},
		{"rounds",    required_argument, 0, 'r'},
		{"seed",      required_argument, 0, 's'},
		{"verbose",   no_argument,       0, 'v'},
		{0,           0,                 0,  0 }
	};

	bool verbose = false;
	uint32_t rounds = 400;
	uint32_t seed = 0;
	uint32_t attackEdges = 0;
	Network::NodeFactory nodeFactory = nullptr;
	uint32_t candidates = 0;
	std::string graphFile;
	while (true) {
		int32_t optionIndex;
		int32_t c = getopt_long(argc, argv, "hg:n:m:r:s:v", long_options, &optionIndex);
		if (c == -1) {
			break;
		}
		switch (c) {
		case 'n':
		{
			std::string option = optarg;
			if (option == "tmr") { // three majority random
				candidates = 1;
				nodeFactory = [](Network* network, uint32_t id){ return new ThreeMajorityNode(network, id); };
			} else if (option.find("borda") == 0) {
				candidates = std::stoi(optarg + 5);
				nodeFactory = [candidates](Network* network, uint32_t id){ return new BordaNode(network, id, candidates); };
			}
			break;
		}
		case 'm':
			attackEdges = std::stoi(optarg);
			break;
		case 'g':
			graphFile = optarg;
			break;
		case 'r':
			rounds = std::stoi(optarg);
			break;
		case 's':
			seed = std::stoi(optarg);
			break;
		case 'v':
			verbose = true;
			break;
		case 'h':
		default:
			printUsage(argv);
			break;
		}
	}

	if (graphFile.size() == 0 || nodeFactory == nullptr) {
		printUsage(argv);
	}

	seedRandomGenerator(seed);
	Network n(graphFile, nodeFactory);
	if (attackEdges != 0) {
		uint32_t maliciousNode = n.addMaliciousNode(attackEdges, candidates);
		if (verbose) {
			printf("Malicious node was %u\n", maliciousNode);
		} else {
			printf("%u\n", maliciousNode);
		}
	}

	for (uint32_t i = 0; i < rounds; ++i) {
		Statistics s = n.stepSimulation();
		if (verbose) {
			printf("After round %u:\n\tleader is %u\n\tagreement ratio is %f\n\tnum leaders is %u\n\tleader change ratio was %f\n", s.m_simulationStep, s.m_leaderId, s.m_agreementRatio, s.m_numLeaders, s.m_leaderChangeRatio);
		} else {
			printf("%u\t%u\t%f\t%u\t%f\n", s.m_simulationStep, s.m_leaderId, s.m_agreementRatio, s.m_numLeaders, s.m_leaderChangeRatio);
		}
	}
	return 0;
}