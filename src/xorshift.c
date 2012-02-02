#include "xorshift.h"
#include <sys/time.h>

#define A 21
#define B 1
#define C 27

static uint64_t g_seed = 0;
static uint8_t unseeded = 1; 

static void set_seed(uint64_t input)
{
  unseeded = 0;
  g_seed = input;
}
static uint64_t get_seed()
{
  if(unseeded) {
    struct timeval tim;
    gettimeofday(&tim, 0);
    g_seed = tim.tv_sec;
    g_seed <<= 32;
    g_seed += tim.tv_usec;
    unseeded = 0;
  }
  return g_seed;
}

void xorshift_srand(uint64_t seed)
{
  set_seed(seed ? seed : 1);
}

uint64_t xorshift_rand(void)
{
  static const uint16_t a = A;
  static const uint16_t b = B;
  static const uint16_t c = C;

  uint64_t y;
  y = get_seed();
  y ^= y<<a;
  
  y ^= y>>b;
  y ^= y<<c;

  set_seed(y);
  return y;
}
