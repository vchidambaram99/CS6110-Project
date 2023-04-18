#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <stdint.h>

void seedRandomGenerator(uint32_t seed);
uint32_t random(uint32_t low, uint32_t high);

#endif // _UTILS_HPP