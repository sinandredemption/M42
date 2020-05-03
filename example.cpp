#include "m42.h"
#include <iostream>
#include <sstream>
using namespace std;

std::string bb_to_str(uint64_t b1, uint64_t b2)
{
  std::ostringstream ss;
  for (int r = 7; r >= 0; --r) {
    ss << "    +---+---+---+---+---+---+---+---+    +---+---+---+---+---+---+---+---+\n";
    ss.width(3);
    ss << r + 1 << " |";
    for (int f = 0; f <= 7; ++f) {
      ss << ((b1 & (1ULL << ((r << 3) | f))) ? " X |" : "   |");
    }
    ss << "    |";
    for (int f = 0; f <= 7; ++f) {
      ss << ((b2 & (1ULL << ((r << 3) | f))) ? " X |" : "   |");
    }
    ss << '\n';
  }
  ss << "    +---+---+---+---+---+---+---+---+    +---+---+---+---+---+---+---+---+\n";
  ss << "      a   b   c   d   e   f   g   h        a   b   c   d   e   f   g   h  \n";
  return ss.str();
}

uint64_t rand64()
{
  static uint64_t s = 3141592653589793238ULL;
  s ^= (s ^= (s ^= s >> 12) << 25) >> 27; /* xorshift64star */
  return s * 0x2545F4914F6CDD1DULL;
}

void pause() {
  cout << "Press enter to continue...";
  cin.get();
}

int main() {
  M42::init();
  
  int i;
  uint64_t b;

  cout << "Welcome to M42, an open-source library for generation of"
    << " Bitboard attack masks.\n This program will interactively"
    << " demonstrate the three most important the functions offered"
    << "by the library.\n\n";
  pause();

  cout << "Function #1: M42::rook_attacks(int sq, uint64_t occ)\n";
  cout << "    This function is used to generate Bitboard attack mask of"
    << " a rook on square 'sq' given the occupancy bitboard 'occ'. It uses"
    << " fancy magic Bitboards to do that, so it's pretty fast.\n";
  pause();
  cout << '\n';
  for (i = 0; i < 5; ++i) {
    int sq = rand64() & 63;
    b = rand64() & rand64();
    cout << "\n" << i + 1 << "/5 - Rook on square '" << char((sq & 7) + 'a') << char((sq >> 3) + '1') << "':\n";
    cout << "             Occupancy Bitboard                   Attacks Bitboard" << endl;
    cout << bb_to_str(b, M42::rook_attacks(sq, b)) << endl;
    pause();
  }

  cout << "\nFunction #2: M42::bishop_attacks(int sq, uint64_t occ)\n";
  cout << "    This function is used to generate Bitboard attack mask of"
    << " a bihsop on square 'sq' given the occupancy bitboard 'occ'. It uses"
    << " fancy magic Bitboards to do that, so it's pretty fast.\n";
  pause();
  cout << '\n';
  for (i = 0; i < 5; ++i) {
    int sq = rand64() & 63;
    b = rand64() & rand64() & rand64();
    cout << "\n" << i+1 << "/5 - Bishop on square '" << char((sq & 7) + 'a') << char((sq >> 3) + '1') << "':\n";
    cout << "             Occupancy Bitboard                   Attacks Bitboard" << endl;
    cout << bb_to_str(b, M42::bishop_attacks(sq, b)) << endl;
    pause();
  }

  cout << "\nFunction #3: M42::queen_attacks(int sq, uint64_t occ)\n";
  cout << "    This function is used to generate Bitboard attack mask of"
    << " a queen on square 'sq' given the occupancy bitboard 'occ'. It uses"
    << " fancy magic Bitboards to do that, so it's pretty fast.\n";
  pause();
  cout << '\n';
  for (i = 0; i < 5; ++i) {
    int sq = rand64() & 63;
    b = rand64() & rand64() & rand64();
    cout << "\n" << i + 1 << "/5 - Queen on square '" << char((sq & 7) + 'a') << char((sq >> 3) + '1') << "':\n";
    cout << "             Occupancy Bitboard                   Attacks Bitboard" << endl;
    cout << bb_to_str(b, M42::queen_attacks(sq, b)) << endl;
    pause();
  }

  cout << "Now read the documentation to know about more function offered by the library.\n";
  cout << "Have a nice day!\n";
  pause();

  return 0;
}