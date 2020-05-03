#include "m42.h"

namespace M42 {
  uint64_t KnightAttacks[64];
  uint64_t KingAttacks[64];
  uint64_t RankMaskEx[64];
  uint64_t FileMaskEx[64];
  uint64_t DiagMaskEx[64];
  uint64_t ADiagMaskEx[64];
  uint64_t PawnAttacks[2][64];
  uint64_t SquareMask[64];
  uint64_t ThisAndNextSq[64];
  uint64_t PrevSquares[64];

  uint64_t RTables[0x16200]; // 708 kB
  uint64_t * RAttacks[64];
  uint64_t RMasks[64];
  uint64_t BTables[0x12C0]; // 37 kB
  uint64_t * BAttacks[64];
  uint64_t BMasks[64];

  // Initialize fancy magic bitboards
  void init_piece(bool rook, int sq)
  {
    uint64_t *Masks = rook ? RMasks : BMasks;
    const uint64_t * Magics = rook ? RMagics : BMagics;
    uint64_t ** Attacks = rook ? RAttacks : BAttacks;
    uint64_t(*calc_attacks)(int, uint64_t) =
      rook ? calc_rook_attacks : calc_bishop_attacks;
    const unsigned * Shift = rook ? RShift : BShift;

    Masks[sq] = calc_attacks(sq, 0) & ~SquareMask[sq];
    if ((sq & 7) != 0)
      Masks[sq] &= ~FileAMask;
    if ((sq & 7) != 7)
      Masks[sq] &= ~FileHMask;
    if ((sq >> 3) != 0)
      Masks[sq] &= ~0x00000000000000FFULL;  // Rank A
    if ((sq >> 3) != 7)
      Masks[sq] &= ~0xFF00000000000000ULL;  // Rank H

    const size_t TableSize = 1ULL << (64 - Shift[sq]);
    std::memset(Attacks[sq], 0, TableSize * sizeof(uint64_t));

    uint64_t occ = 0;
    do {
      uint32_t index = uint32_t(((occ & Masks[sq]) * Magics[sq]) >> Shift[sq]);
      assert((Attacks[sq][index] == 0)
        || (Attacks[sq][index] == calc_attacks(sq, occ)));
      Attacks[sq][index] = calc_attacks(sq, occ);
    } while (occ = next_subset(Masks[sq], occ));

    if (sq < 63)
      Attacks[sq + 1] = Attacks[sq] + TableSize;
  }

  void init()
  {
    int sq;

    const uint64_t RankMask[] = {
      0x00000000000000FFULL, 0x000000000000FF00ULL, 0x0000000000FF0000ULL,
      0x00000000FF000000ULL, 0x000000FF00000000ULL, 0x0000FF0000000000ULL,
      0x00FF000000000000ULL, 0xFF00000000000000ULL
    };
    const uint64_t FileMask[] = {
      0x0101010101010101ULL, 0x0202020202020202ULL, 0x0404040404040404ULL,
      0x0808080808080808ULL, 0x1010101010101010ULL, 0x2020202020202020ULL,
      0x4040404040404040ULL, 0x8080808080808080ULL
    };
    const uint64_t DiagMask[] = {
      0x8040201008040201ULL, 0x4020100804020100ULL, 0x2010080402010000ULL,
      0x1008040201000000ULL, 0x0804020100000000ULL, 0x0402010000000000ULL,
      0x0201000000000000ULL, 0x0100000000000000ULL,
      0x0000000000000000ULL,
      0x0000000000000080ULL, 0x0000000000008040ULL, 0x0000000000804020ULL,
      0x0000000080402010ULL, 0x0000008040201008ULL, 0x0000804020100804ULL,
      0x0080402010080402ULL
    };
    const uint64_t ADiagMask[] = {
      0x0102040810204080ULL, 0x0001020408102040ULL, 0x0000010204081020ULL,
      0x0000000102040810ULL, 0x0000000001020408ULL, 0x0000000000010204ULL,
      0x0000000000000102ULL, 0x0000000000000001ULL,
      0x0000000000000000ULL,
      0x8000000000000000ULL, 0x4080000000000000ULL, 0x2040800000000000ULL,
      0x1020408000000000ULL, 0x0810204080000000ULL, 0x0408102040800000ULL,
      0x0204081020408000ULL
    };

    for (sq = 0; sq < 64; ++sq) {
      SquareMask[sq] = 1ULL << sq;
      ThisAndNextSq[sq] = 3ULL << sq;
      PrevSquares[sq] = ((1ULL << sq) - 1) + (sq == 0);

      RankMaskEx[sq] = RankMask[sq >> 3] & ~SquareMask[sq];
      FileMaskEx[sq] = FileMask[sq & 7] & ~SquareMask[sq];
      DiagMaskEx[sq] = DiagMask[((sq >> 3) - (sq & 7)) & 15] & ~SquareMask[sq];
      ADiagMaskEx[sq] = ADiagMask[((sq >> 3) + (sq & 7)) ^ 7] & ~SquareMask[sq];
    }

    // Initialize all "fancy" magic bitboards
    RAttacks[0] = RTables;  // Set first offset
    BAttacks[0] = BTables;  // Set first offset

    for (sq = 0; sq < 64; ++sq) {
      init_piece(true, sq);
      init_piece(false, sq);

      KingAttacks[sq] = calc_king_attacks(SquareMask[sq]);
      KnightAttacks[sq] = calc_knight_attacks(SquareMask[sq]);

      // Initialize pawn attacks
      PawnAttacks[0][sq] = calc_pawn_attacks<0>(SquareMask[sq]);
      PawnAttacks[1][sq] = calc_pawn_attacks<1>(SquareMask[sq]);
    }
  }
}