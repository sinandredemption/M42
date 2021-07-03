#include <cassert>
#include <bit>
#include <bitset>
#include <iostream>
#include <random>

#include <m42.h>
#include <m42v2.h>

int main() {
  std::random_device rd; //seed
  std::mt19937 gen(rd()); //seed for rd(Mersenne twister)
  auto distribution = std::uniform_int_distribution<uint64_t>(0ULL, UINT64_MAX);
  size_t i = 0;
  while(1) {
    uint64_t x = distribution(gen);
    if(M42v2::msb(x) != M42::msb(x)) {
      std::cout << i << " mitmatch: bit_width(" << std::bitset<64>{x} << ") = " << M42v2::msb(x) << " " << M42::msb(x) << '\n';
      abort();
    }
    if(i == int(1e7)) {
      std::cout << "bit_width( " << std::bitset<64>{x} << " ) = " << M42v2::msb(x) << " " << M42::msb(x) << '\n';
      i = 0;
    }
    ++i;
  }
}
