#pragma once

#include <iostream>
#include <bitset>

#include <m42.h>
#include <m42v2.h>


namespace M42_proxy {
  inline uint64_t file_attacks(int sq, uint64_t occ) {
    const uint64_t v1 = M42::file_attacks(sq, occ);
    const uint64_t v2 = M42v2::file_attacks(sq, occ);
    if(v1 != v2) {
      std::cout << "file_attacks: occ " << std::bitset<64>{occ} << " dec " << occ << " sq " << sq << std::endl;
      std::cout << "      " << std::bitset<64>{v1} << std::endl;
      std::cout << "      " << std::bitset<64>{v2} << std::endl;
    }
    return v2;
  }

  inline uint64_t rank_attacks(int sq, uint64_t occ) {
    const uint64_t v1 = M42::rank_attacks(sq, occ);
    const uint64_t v2 = M42v2::rank_attacks(sq, occ);
    if(v1 != v2) {
      std::cout << "rank_attacks: occ " << std::bitset<64>{occ} << " dec " << occ << " sq " << sq << std::endl;
      std::cout << "      " << std::bitset<64>{v1} << std::endl;
      std::cout << "      " << std::bitset<64>{v2} << std::endl;
    }
    return v2;
  }
} // namespace M42_proxy
