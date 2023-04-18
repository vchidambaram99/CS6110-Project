#include <random>
#include "common/Utils.hpp"

std::default_random_engine engine;

void seedRandomGenerator(uint32_t seed) {
	engine = std::default_random_engine({seed});
}

uint32_t random(uint32_t low, uint32_t high) {
	return std::uniform_int_distribution<uint32_t>(low, high)(engine);
}