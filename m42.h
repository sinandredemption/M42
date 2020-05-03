/* M42 - A small library for Bitboard attack mask generation, by Syed Fahad
 *
 * Usage: Just include this file as a normal header, and m42.cpp into your
 * source code, and then call M42::init() at your program startup.
 * THIS IS IMPORTANT, you've to call M42::init() in the main() function of
 * your program, to use this library.
 * Read the documentation to know what every function does.
 *
 * Copyright: NO COPYRIGHT. Yeah, that's right, I give away all the
 * copyrights of this library. So you can use this code and/or modify as
 * you like. You're free to do whatever you want, though I'd appreciate
 * if you'd mention the library name and source in case you use this
 * library.
 *
 * My email: sydfhd at gmail dot com. Do drop me a line in case you
 * improve this library, or add more functions to it.
 */

#ifndef INC_M42_H_
#define INC_M42_H_
#include <inttypes.h>
#include <cassert>
#include <cstdlib>
#include <cstring>

namespace M42 {
  extern uint64_t KnightAttacks[64];
  extern uint64_t KingAttacks[64];
  extern uint64_t RankMaskEx[64];
  extern uint64_t FileMaskEx[64];
  extern uint64_t DiagMaskEx[64];
  extern uint64_t ADiagMaskEx[64];
  extern uint64_t PawnAttacks[2][64];
  extern uint64_t SquareMask[64];
  extern uint64_t ThisAndNextSq[64];
  extern uint64_t PrevSquares[64];

  extern uint64_t RTables[0x16200]; // 708 kB
  extern uint64_t * RAttacks[64];
  extern uint64_t RMasks[64];
  extern uint64_t BTables[0x12C0]; // 37 kB
  extern uint64_t * BAttacks[64];
  extern uint64_t BMasks[64];

  const uint64_t FileAMask = 0x0101010101010101ULL;
  const uint64_t FileHMask = 0x8080808080808080ULL;
  const unsigned RShift[64] = {
    52, 53, 53, 53, 53, 53, 53, 52,
    53, 54, 54, 53, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 53, 54, 54, 54, 53,
    54, 55, 55, 55, 55, 55, 54, 54,
    53, 54, 54, 54, 54, 53, 54, 53,
  };

  // Very dense and high quality magics taken from Texel
  const uint64_t RMagics[64] = {
    0x19A80065FF2BFFFFULL, 0x3FD80075FFEBFFFFULL, 0x4010000DF6F6FFFEULL,
    0x0050001FAFFAFFFFULL, 0x0050028004FFFFB0ULL, 0x7F600280089FFFF1ULL,
    0x7F5000B0029FFFFCULL, 0x5B58004848A7FFFAULL, 0x002A90005547FFFFULL,
    0x000050007F13FFFFULL, 0x007FA0006013FFFFULL, 0x006A9005656FFFFFULL,
    0x007F600F600AFFFFULL, 0x007EC007E6BFFFE2ULL, 0x007EC003EEBFFFFBULL,
    0x0071D002382FFFDAULL, 0x009F803000E7FFFAULL, 0x00680030008BFFFFULL,
    0x00606060004F3FFCULL, 0x001A00600BFF9FFDULL, 0x000D006005FF9FFFULL,
    0x0001806003005FFFULL, 0x00000300040BFFFAULL, 0x000192500065FFEAULL,
    0x00FFF112D0006800ULL, 0x007FF037D000C004ULL, 0x003FD062001A3FF8ULL,
    0x00087000600E1FFCULL, 0x000FFF0100100804ULL, 0x0007FF0100080402ULL,
    0x0003FFE0C0060003ULL, 0x0001FFD53000D300ULL, 0x00FFFD3000600061ULL,
    0x007FFF7F95900040ULL, 0x003FFF8C00600060ULL, 0x001FFE2587A01860ULL,
    0x000FFF3FBF40180CULL, 0x0007FFC73F400C06ULL, 0x0003FF86D2C01405ULL,
    0x0001FFFEAA700100ULL, 0x00FFFDFDD8005000ULL, 0x007FFF80EBFFB000ULL,
    0x003FFFDF603F6000ULL, 0x001FFFE050405000ULL, 0x000FFF400700C00CULL,
    0x0007FF6007BF600AULL, 0x0003FFEEBFFEC005ULL, 0x0001FFFDF3FEB001ULL,
    0x00FFFF39FF484A00ULL, 0x007FFF3FFF486300ULL, 0x003FFF99FFAC2E00ULL,
    0x001FFF31FF2A6A00ULL, 0x000FFF19FF15B600ULL, 0x0007FFF5FFF28600ULL,
    0x0003FFFDDFFBFEE0ULL, 0x0001FFF5F63C96A0ULL, 0x00FFFF5DFF65CFB6ULL,
    0x007FFFBAFFD1C5AEULL, 0x003FFF71FF6CBCEAULL, 0x001FFFD9FFD4756EULL,
    0x000FFFF5FFF338E6ULL, 0x0007FFFDFFFE24F6ULL, 0x0003FFEF27EEBE74ULL,
    0x0001FFFF23FF605EULL
  };

  const unsigned BShift[64] = {
    59, 60, 59, 59, 59, 59, 60, 59,
    60, 60, 59, 59, 59, 59, 60, 60,
    60, 60, 57, 57, 57, 57, 60, 60,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    60, 60, 57, 57, 57, 57, 60, 60,
    60, 60, 59, 59, 59, 59, 60, 60,
    59, 60, 59, 59, 59, 59, 60, 59,
  };
  const uint64_t BMagics[64] = {
    0x0006EFF5367FF600ULL, 0x00345835BA77FF2BULL, 0x00145F68A3F5DAB6ULL,
    0x003A1863FB56F21DULL, 0x0012EB6BFE9D93CDULL, 0x000D82827F3420D6ULL,
    0x00074BCD9C7FEC97ULL, 0x000034FE99F9FFFFULL, 0x0000746F8D6717F6ULL,
    0x00003ACB32E1A3F7ULL, 0x0000185DAF1FFB8AULL, 0x00003A1867F17067ULL,
    0x0000038EE0CCF92EULL, 0x000002A2B7FF926EULL, 0x000006C9AA93FF14ULL,
    0x00000399B5E5BF87ULL, 0x00400F342C951FFCULL, 0x0020230579ED8FF0ULL,
    0x007B008A0077DBFDULL, 0x001D00010C13FD46ULL, 0x00040022031C1FFBULL,
    0x000FA00FD1CBFF79ULL, 0x000400A4BC9AFFDFULL, 0x000200085E9CFFDAULL,
    0x002A14560A3DBFBDULL, 0x000A0A157B9EAFD1ULL, 0x00060600FD002FFAULL,
    0x004006000C009010ULL, 0x001A002042008040ULL, 0x001A00600FD1FFC0ULL,
    0x000D0ACE50BF3F8DULL, 0x000183A48434EFD1ULL, 0x001FBD7670982A0DULL,
    0x000FE24301D81A0FULL, 0x0007FBF82F040041ULL, 0x000040C800008200ULL,
    0x007FE17018086006ULL, 0x003B7DDF0FFE1EFFULL, 0x001F92F861DF4A0AULL,
    0x000FD713AD98A289ULL, 0x000FD6AA751E400CULL, 0x0007F2A63AE9600CULL,
    0x0003FF7DFE0E3F00ULL, 0x000003FD2704CE04ULL, 0x00007FC421601D40ULL,
    0x007FFF5F70900120ULL, 0x003FA66283556403ULL, 0x001FE31969AEC201ULL,
    0x0007FDFC18AC14BBULL, 0x0003FB96FB568A47ULL, 0x000003F72EA4954DULL,
    0x00000003F8DC0383ULL, 0x0000007F3A814490ULL, 0x00007DC5C9CF62A6ULL,
    0x007F23D3342897ACULL, 0x003FEE36EEE1565CULL, 0x0003FF3E99FCCCC7ULL,
    0x000003ECFCFAC5FEULL, 0x00000003F97F7453ULL, 0x0000000003F8DC03ULL,
    0x000000007EFA8146ULL, 0x0000007ED3E2EF60ULL, 0x00007F47243ADCD6ULL,
    0x007FB65AFABFB3B5ULL,
  };

  inline int msb(uint64_t b)
  {
    const int BitScanTable[64] = {
      0, 47,  1, 56, 48, 27,  2, 60,
      57, 49, 41, 37, 28, 16,  3, 61,
      54, 58, 35, 52, 50, 42, 21, 44,
      38, 32, 29, 23, 17, 11,  4, 62,
      46, 55, 26, 59, 40, 36, 15, 53,
      34, 51, 20, 43, 31, 22, 10, 45,
      25, 39, 14, 33, 19, 30,  9, 24,
      13, 18,  8, 12,  7,  6,  5, 63
    };
    b |= (b |= (b |= (b |= (b |= b >> 1) >> 2) >> 4) >> 8) >> 16;
    return BitScanTable[((b | b >> 32) * 0x03f79d71b4cb0a89ULL) >> 58];
  }

  inline uint64_t byteswap(uint64_t b)
  {
#ifdef USE_INTRIN
    return _byteswap_uint64(b);
#else
    b = ((b >> 8) & 0x00FF00FF00FF00FFULL) | ((b & 0x00FF00FF00FF00FFULL) << 8);
    b = ((b >> 16) & 0x0000FFFF0000FFFFULL) | ((b & 0x0000FFFF0000FFFFULL) << 16);
    return (b >> 32) | (b << 32);
#endif
  }

  inline uint64_t king_attacks(int sq) {
    return KingAttacks[sq];
  }

  inline uint64_t calc_king_attacks(uint64_t kings)
  {
    uint64_t attacks = ((kings << 1) &~FileAMask) | ((kings >> 1) &~FileHMask);
    kings |= attacks;
    return attacks | (kings << 8) | (kings >> 8);
  }

  inline uint64_t knight_attacks(int sq) {
    return KnightAttacks[sq];
  }

  inline uint64_t calc_knight_attacks(uint64_t knights)
  {
    uint64_t const h1 = ((knights >> 1) & 0x7F7F7F7F7F7F7F7FULL)
      | ((knights << 1) & 0xFEFEFEFEFEFEFEFEULL);
    uint64_t const h2 = ((knights >> 2) & 0x3F3F3F3F3F3F3F3FULL)
      | ((knights << 2) & 0xFCFCFCFCFCFCFCFCULL);
    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
  }

  inline uint64_t pawn_attacks(unsigned cl, int sq) {
    return PawnAttacks[cl][sq];
  }

  template <unsigned Cl>
  inline uint64_t calc_pawn_attacks(uint64_t pawns)
  {
    if (Cl == 0) {  // White
      return ((pawns << 7) &~FileHMask) | ((pawns << 9) &~FileAMask);
    }
    else if (Cl == 1) {  // Black
      return ((pawns >> 7) &~FileAMask) | ((pawns >> 9) &~FileHMask);
    }
    else {
      assert(false);
      return 0;
    }
  }

  inline uint64_t rank_attacks(int sq, uint64_t occ)
  {
    static const uint64_t OuterSquares[64] = {
      0x0000000000000081ULL, 0x0000000000000081ULL, 0x0000000000000081ULL,
      0x0000000000000081ULL, 0x0000000000000081ULL, 0x0000000000000081ULL,
      0x0000000000000081ULL, 0x0000000000000001ULL, 0x0000000000008000ULL,
      0x0000000000008100ULL, 0x0000000000008100ULL, 0x0000000000008100ULL,
      0x0000000000008100ULL, 0x0000000000008100ULL, 0x0000000000008100ULL,
      0x0000000000000100ULL, 0x0000000000800000ULL, 0x0000000000810000ULL,
      0x0000000000810000ULL, 0x0000000000810000ULL, 0x0000000000810000ULL,
      0x0000000000810000ULL, 0x0000000000810000ULL, 0x0000000000010000ULL,
      0x0000000080000000ULL, 0x0000000081000000ULL, 0x0000000081000000ULL,
      0x0000000081000000ULL, 0x0000000081000000ULL, 0x0000000081000000ULL,
      0x0000000081000000ULL, 0x0000000001000000ULL, 0x0000008000000000ULL,
      0x0000008100000000ULL, 0x0000008100000000ULL, 0x0000008100000000ULL,
      0x0000008100000000ULL, 0x0000008100000000ULL, 0x0000008100000000ULL,
      0x0000000100000000ULL, 0x0000800000000000ULL, 0x0000810000000000ULL,
      0x0000810000000000ULL, 0x0000810000000000ULL, 0x0000810000000000ULL,
      0x0000810000000000ULL, 0x0000810000000000ULL, 0x0000010000000000ULL,
      0x0080000000000000ULL, 0x0081000000000000ULL, 0x0081000000000000ULL,
      0x0081000000000000ULL, 0x0081000000000000ULL, 0x0081000000000000ULL,
      0x0081000000000000ULL, 0x0001000000000000ULL, 0x8000000000000000ULL,
      0x8100000000000000ULL, 0x8100000000000000ULL, 0x8100000000000000ULL,
      0x8100000000000000ULL, 0x8100000000000000ULL, 0x8100000000000000ULL,
      0x0100000000000000ULL
    };
    occ = (occ & RankMaskEx[sq]) | OuterSquares[sq];
    return ((occ - ThisAndNextSq[msb(occ & PrevSquares[sq])]) ^ occ)
      & RankMaskEx[sq];
  }

  inline uint64_t file_attacks(int sq, uint64_t occ)
  {
    static const uint64_t OuterSquares[64] = {
      0x0100000000000000ULL, 0x0200000000000000ULL, 0x0400000000000000ULL,
      0x0800000000000000ULL, 0x1000000000000000ULL, 0x2000000000000000ULL,
      0x4000000000000000ULL, 0x8000000000000000ULL, 0x0100000000000001ULL,
      0x0200000000000002ULL, 0x0400000000000004ULL, 0x0800000000000008ULL,
      0x1000000000000010ULL, 0x2000000000000020ULL, 0x4000000000000040ULL,
      0x8000000000000080ULL, 0x0100000000000001ULL, 0x0200000000000002ULL,
      0x0400000000000004ULL, 0x0800000000000008ULL, 0x1000000000000010ULL,
      0x2000000000000020ULL, 0x4000000000000040ULL, 0x8000000000000080ULL,
      0x0100000000000001ULL, 0x0200000000000002ULL, 0x0400000000000004ULL,
      0x0800000000000008ULL, 0x1000000000000010ULL, 0x2000000000000020ULL,
      0x4000000000000040ULL, 0x8000000000000080ULL, 0x0100000000000001ULL,
      0x0200000000000002ULL, 0x0400000000000004ULL, 0x0800000000000008ULL,
      0x1000000000000010ULL, 0x2000000000000020ULL, 0x4000000000000040ULL,
      0x8000000000000080ULL, 0x0100000000000001ULL, 0x0200000000000002ULL,
      0x0400000000000004ULL, 0x0800000000000008ULL, 0x1000000000000010ULL,
      0x2000000000000020ULL, 0x4000000000000040ULL, 0x8000000000000080ULL,
      0x0100000000000001ULL, 0x0200000000000002ULL, 0x0400000000000004ULL,
      0x0800000000000008ULL, 0x1000000000000010ULL, 0x2000000000000020ULL,
      0x4000000000000040ULL, 0x8000000000000080ULL, 0x0000000000000001ULL,
      0x0000000000000002ULL, 0x0000000000000004ULL, 0x0000000000000008ULL,
      0x0000000000000010ULL, 0x0000000000000020ULL, 0x0000000000000040ULL,
      0x0000000000000080ULL
    };

    occ = (occ & FileMaskEx[sq]) | OuterSquares[sq];
    return ((occ - ThisAndNextSq[msb(occ & PrevSquares[sq])]) ^ occ)
      & FileMaskEx[sq];
  }

  inline uint64_t calc_rook_attacks(int sq, uint64_t occ)
  {
    return file_attacks(sq, occ) | rank_attacks(sq, occ);
  }

  // Use Hyperbola Quintessence for bishop attack generation
  inline uint64_t diag_attacks(int sq, uint64_t occ)
  {
    occ &= DiagMaskEx[sq];
    return ((occ - SquareMask[sq]) ^ byteswap(byteswap(occ) - SquareMask[sq ^ 56]))
      & DiagMaskEx[sq];
  }

  inline uint64_t adiag_attacks(int sq, uint64_t occ)
  {
    occ &= ADiagMaskEx[sq];
    return ((occ - SquareMask[sq]) ^ byteswap(byteswap(occ) - SquareMask[sq ^ 56]))
      & ADiagMaskEx[sq];
  }

  inline uint64_t calc_bishop_attacks(int sq, uint64_t occ)
  {
    return diag_attacks(sq, occ) | adiag_attacks(sq, occ);
  }

  inline uint64_t rook_attacks(int sq, uint64_t occ) {
    return RAttacks[sq][((occ & RMasks[sq]) * RMagics[sq]) >> RShift[sq]];
  }

  inline uint64_t bishop_attacks(int sq, uint64_t occ) {
    return BAttacks[sq][((occ & BMasks[sq]) * BMagics[sq]) >> BShift[sq]];
  }

  inline uint64_t queen_attacks(int sq, uint64_t occ) {
    return rook_attacks(sq, occ) | bishop_attacks(sq, occ);
  }

  inline uint64_t next_subset(const uint64_t Set, const uint64_t subset)
  {
    return (subset - Set) & Set;
  }

  void init();
  void init_piece(bool rook, int sq);
}
#endif