#ifndef XORSHIFT_H
#define XORSHIFT_H
#include <stdint.h>

void xorshift_srand(uint64_t seed);
uint64_t xorshift_rand(void);

#endif
