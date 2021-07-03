#pragma once


#include <cmath>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <climits>
#include <strings.h>

#include <bit>
#include <array>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_set>

#include <m42_proxy.h>

using namespace std::string_literals;

class Perft;

// type alias: position index on bitboard
typedef uint8_t pos_t;
typedef uint16_t pos_pair_t;

// https://graphics.stanford.edu/~seander/bithacks.html
// bit-hacks from stanford graphics, to be placed here

// https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
// https://en.cppreference.com/w/cpp/header/bit
namespace bitmask {
  const uint64_t full = ~UINT64_C(0);
  const uint64_t vline = UINT64_C(0x0101010101010101);
  const uint64_t hline = UINT64_C(0xFF);

  inline constexpr pos_pair_t _pos_pair(pos_t a, pos_t b) {
    return pos_pair_t(a) << 8 | b;
  }

  inline constexpr pos_t first(pos_pair_t p) {
    return p >> 8;
  }

  inline constexpr pos_t second(pos_pair_t p) {
    return (pos_t)p;
  }

  template <typename T>
  inline constexpr bool is_exp2(T v) {
    return std::has_single_bit(v);
//    return !(v & (v - 1));
  }

  template <typename T>
  inline constexpr pos_t log2_of_exp2(T v) {
    return std::countr_zero(v);
//    pos_t r = 0x00;
//    if (v >= (UINT64_C(1) << 32)) { r += 32; v >>= 32; }
//    if (v >= (UINT64_C(1) << 16)) { r += 16; v >>= 16; }
//    if (v >= (UINT64_C(1) << 8 )) { r += 8 ; v >>= 8 ; }
//    if (v >= (UINT64_C(1) << 4 )) { r += 4 ; v >>= 4 ; }
//    if (v >= (UINT64_C(1) << 2 )) { r += 2 ; v >>= 2 ; }
//    if (v >= (UINT64_C(1) << 1 )) { r += 1 ; v >>= 1 ; }
//    return r;
  }

  template <typename T>
  inline constexpr pos_t count_bits(T v) {
    return std::popcount(v);
//    if(!v)return 0;
//    if(bitmask::is_exp2(v))return 1;
//    v = v - ((v >> 1) & (T)~(T)0/3);                           // temp
//    v = (v & (T)~(T)0/15*3) + ((v >> 2) & (T)~(T)0/15*3);      // temp
//    v = (v + (v >> 4)) & (T)~(T)0/255*15;                      // temp
//    return (T)(v * ((T)~(T)0/255)) >> (sizeof(T) - 1) * CHAR_BIT; // count
  }

  template <typename T>
  inline constexpr pos_t log2(T t) {
    return std::bit_width(t);
//    pos_t shift = 0;
//    pos_t r = 0;
//    r =     (t > 0xFFFFFFFFLLU)?1<<5:0; t >>= r;
//    shift = (t > 0xFFFF)       ?1<<4:0; t >>= shift; r |= shift;
//    shift = (t > 0xFF)         ?1<<3:0; t >>= shift; r |= shift;
//    shift = (t > 0xF)          ?1<<2:0; t >>= shift; r |= shift;
//    shift = (t > 0x3)          ?1<<1:0; t >>= shift; r |= shift;
//                                                     r |= (t >> 1);
//    return r;
  }

  // https://www.chessprogramming.org/BitScan#DeBruijnMultiplation
  inline constexpr uint64_t lowest_bit(uint64_t v) {
    return 1ULL << std::countr_zero(v);
//    if(!v)return 0ULL;
//    constexpr int index64[64] = {
//        0, 47,  1, 56, 48, 27,  2, 60,
//       57, 49, 41, 37, 28, 16,  3, 61,
//       54, 58, 35, 52, 50, 42, 21, 44,
//       38, 32, 29, 23, 17, 11,  4, 62,
//       46, 55, 26, 59, 40, 36, 15, 53,
//       34, 51, 20, 43, 31, 22, 10, 45,
//       25, 39, 14, 33, 19, 30,  9, 24,
//       13, 18,  8, 12,  7,  6,  5, 63
//    };
//    const uint64_t debruijn64 = UINT64_C(0x03f79d71b4cb0a89);
//    assert(v);
//    return 1ULL << index64[((v ^ (v-1)) * debruijn64) >> 58];
  }

  template <typename T>
  inline constexpr T highest_bit(T v) {
    return std::bit_floor(v);
//    if(!v)return 0ULL;
//    v |= (v >>  1);
//    v |= (v >>  2);
//    v |= (v >>  4);
//    v |= (v >>  8);
//    v |= (v >> 16);
//    v |= (v >> 32);
//    return v - (v >> 1);
  }

  template <typename T>
  inline constexpr T ones_before_eq_bit(T v) {
    if(!v)return UINT64_MAX;
    assert(bitmask::is_exp2(v));
    return (v - 1) << 1;
  }

  template <typename T>
  inline constexpr T ones_before_bit(T v) {
    return ones_before_eq_bit(v >> 1);
  }

  template <typename T>
  inline constexpr T ones_after_eq_bit(T v) {
    if(!v)return UINT64_MAX;
    assert(bitmask::is_exp2(v));
    return ~(v - 1);
  }

  template <typename T>
  inline constexpr T ones_after_bit(T v) {
    return ones_after_eq_bit(v << 1);
  }

  // iterate set bits with a function F
  template <typename F>
  inline constexpr void foreach(uint64_t mask, F &&func) {
    if(!mask)return;
    while(mask) {
      const pos_t r = std::countr_zero(mask);
      func(r);
      // unset r-th bit
      assert(mask & (1ULL << r));
      mask &= ~(1LLU << r);
    }
  }

  // iterate set bits with a function F
  template <typename F>
  inline constexpr void foreach_early_stop(uint64_t mask, F &&func) {
    if(!mask)return;
    while(mask) {
      const pos_t r = std::countr_zero(mask);
      if(!func(r)) {
        break;
      }
      // unset r-th bit
      assert(mask & (1ULL << r));
      mask &= ~(1LLU << r);
    }
  }

  std::vector<pos_t> as_vector(uint64_t mask) {
    std::vector<pos_t> v(0, count_bits(mask));
    foreach(mask, [&](pos_t i) mutable -> void {
      v.emplace_back(i);
    });
    return v;
  }

  // print locations of each set bit
  void print(uint64_t mask) {
    foreach(mask, [](pos_t p) mutable -> void {
      printf("(%c, %c)\n", 'A' + (p%8), '1' + (p/8));
    });
  }

  void print_mask(uint64_t mask, pos_t markspot=0xff) {
    printf("mask: %lx\n", mask);
    char s[256];
    pos_t j = 0;
    for(pos_t i = 0; i < CHAR_BIT*sizeof(mask); ++i) {
      pos_t x = i % 8, y = i / 8;
      y = 8 - y - 1;
      const pos_t ind = y * 8 + x;
      if(i == markspot) {
        s[j++] = 'x';
      } else {
        s[j++] = (mask & (1LLU << ind)) ? '*' : '.';
      }
      s[j++] = ' ';
      if(i % CHAR_BIT == 7) {
        s[j++] = '\n';
      }
    }
    s[j] = '\0';
    puts(s);
  }
} // namespace bitmask


// pieces
typedef enum { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY, NO_PIECES=EMPTY } PIECE;

// two bytes, one for each move; second byte may contain information about promotion (2 bits)
typedef pos_pair_t move_t;
typedef uint16_t ply_index_t;

// type alias: bitboard (as a mask)
typedef uint64_t piece_bitboard_t;

// player color, neutral when neither/both
typedef enum { WHITE, BLACK, NEUTRAL, NO_COLORS=NEUTRAL, BOTH } COLOR;

inline constexpr COLOR enemy_of(COLOR c) {
  if(c == NEUTRAL)return NEUTRAL;
  return (c == WHITE) ? BLACK : WHITE;
}

typedef enum : pos_t { KING_SIDE, QUEEN_SIDE } CASTLING_SIDE;

enum {
  A,B,C,D,E,F,G,H
} BOARD_FILE;

/*! \enum
 *
 *  Detailed description
 */
typedef enum : pos_t {
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8,
} BOARD_CELL;

namespace board {
  constexpr pos_t LEN = 8;
  constexpr pos_t SIZE = LEN*LEN;
  constexpr pos_t MOVEMASK = 0x3f;
  constexpr pos_t PROMOTE_KNIGHT = 0<<6,
                  PROMOTE_BISHOP = 1<<6,
                  PROMOTE_ROOK = 2<<6,
                  PROMOTE_QUEEN = 3<<6;
  constexpr move_t nomove = bitmask::_pos_pair(0xff, 0xff);
  constexpr ply_index_t nocastlings = ~ply_index_t(0);
  constexpr pos_t CASTLING_K_WHITE = 0,
                  CASTLING_Q_WHITE = 1,
                  CASTLING_K_BLACK = 2,
                  CASTLING_Q_BLACK = 3;
  constexpr pos_t NO_PIECE_INDICES = int(NO_PIECES)*int(NO_COLORS) + 1;

  inline PIECE get_promotion_as(pos_t j) {
    switch(j & ~board::MOVEMASK) {
      case board::PROMOTE_KNIGHT:return KNIGHT;
      case board::PROMOTE_BISHOP:return BISHOP;
      case board::PROMOTE_ROOK:return ROOK;
      case board::PROMOTE_QUEEN:return QUEEN;
    }
    return PAWN;
  }

  inline constexpr pos_t _castling_index(COLOR c, CASTLING_SIDE side) {
    if(c==WHITE && side==KING_SIDE)return CASTLING_K_WHITE;
    if(c==WHITE && side==QUEEN_SIDE)return CASTLING_Q_WHITE;
    if(c==BLACK && side==KING_SIDE)return CASTLING_K_BLACK;
    if(c==BLACK && side==QUEEN_SIDE)return CASTLING_Q_BLACK;
    return 0xff;
  }

  inline constexpr pos_t _x(pos_t i) {
    return i % LEN;
  }

  inline constexpr pos_t _y(pos_t i) {
    return i / LEN;
  }

  inline constexpr pos_t _pos(pos_t i, pos_t j) {
    return i + (j - 1) * LEN;
  }

  inline constexpr pos_t file_mask(pos_t x) {
    return bitmask::vline << x;
  }

  inline constexpr pos_t rank_mask(pos_t y) {
    return bitmask::hline << (y * board::LEN);
  }

  std::string _pos_str(pos_t i) {
    std::string p;
    i &= board::MOVEMASK;
    p += 'a' + board::_x(i);
    p += '1' + board::_y(i);
    return p;
  }
} // namespace board


namespace piece {
  inline constexpr piece_bitboard_t pos_mask(pos_t k) {
    return 1ULL << k;
  }

  inline constexpr piece_bitboard_t rank_mask(pos_t r) {
    return bitmask::hline << (r * board::LEN);
  }

  inline constexpr piece_bitboard_t file_mask(pos_t f) {
    return bitmask::vline << f;
  }

  inline constexpr bool is_set(piece_bitboard_t mask, pos_t b) {
    return mask & pos_mask(b);
  }

  inline void set_pos(piece_bitboard_t &mask, pos_t b) {
//    assert(!is_set(mask, b));
    mask |= pos_mask(b);
  }

  inline void unset_pos(piece_bitboard_t &mask, pos_t b) {
//    assert(is_set(mask, b));
    mask &= ~pos_mask(b);
  }

  inline void move_pos(piece_bitboard_t &mask, pos_t i, pos_t j) {
    unset_pos(mask, i);
    set_pos(mask, j);
  }

  inline constexpr size_t size(piece_bitboard_t mask) {
    return bitmask::count_bits(mask);
  }


  inline piece_bitboard_t get_pawn_attack(pos_t pos, COLOR c) {
    return M42::pawn_attacks(c, pos);
  }

  inline piece_bitboard_t get_pawn_attacks(piece_bitboard_t mask, COLOR c) {
    constexpr piece_bitboard_t left = piece::file_mask(A);
    constexpr piece_bitboard_t right = piece::file_mask(H);
    constexpr piece_bitboard_t mid = ~(left | right);
    if(c == WHITE) {
      return ((mask & left ) << (board::LEN + 1))
           | ((mask & mid  ) << (board::LEN + 1))
           | ((mask & mid  ) << (board::LEN - 1))
           | ((mask & right) << (board::LEN - 1));
    } else {
      return ((mask & left ) >> (board::LEN - 1))
           | ((mask & mid  ) >> (board::LEN - 1))
           | ((mask & mid  ) >> (board::LEN + 1))
           | ((mask & right) >> (board::LEN + 1));
    }
  }

  inline piece_bitboard_t get_pawn_push_moves(COLOR c, pos_t i, piece_bitboard_t occupied) {
    if(c == WHITE) {
      const piece_bitboard_t pushes = (piece::pos_mask(i) << board::LEN) & ~occupied;
      return (pushes | ((pushes & piece::rank_mask(-1+3)) << board::LEN)) & ~occupied;
    } else {
      const piece_bitboard_t pushes = (piece::pos_mask(i) >> board::LEN) & ~occupied;
      return (pushes | ((pushes & piece::rank_mask(-1+6)) >> board::LEN)) & ~occupied;
    }
  }

  inline bool is_pawn_double_push(COLOR c, pos_t i, pos_t j) {
    return (c == WHITE ? j - i : i - j) == 2*board::LEN;
  }

  inline bool is_pawn_promotion_move(COLOR c, pos_t i, pos_t j) {
    return board::_y(j) == -1 + ((c == WHITE) ? 8 : 1);
  }

  inline pos_t get_pawn_enpassant_trace(COLOR c, pos_t i, pos_t j) {
    assert(is_pawn_double_push(c, i, j));
    return j + ((c == WHITE) ? -board::LEN : board::LEN);
  }


  inline piece_bitboard_t get_sliding_diag_attack(pos_t pos, piece_bitboard_t occupied) {
    return M42::bishop_attacks(pos, occupied);
  }

  inline piece_bitboard_t get_sliding_diag_xray_attack(pos_t pos, piece_bitboard_t friends, piece_bitboard_t foes) {
    const piece_bitboard_t blockers = get_sliding_diag_attack(pos, friends|foes) & foes;
    return get_sliding_diag_attack(pos, friends | (foes ^ blockers));
  }

  inline piece_bitboard_t get_sliding_diag_attacking_ray(pos_t i, pos_t j, piece_bitboard_t occupied) {
    const piece_bitboard_t attacked_bit = piece::pos_mask(j);
    const pos_t x_i=board::_x(i), y_i=board::_y(i),
                x_j=board::_x(j), y_j=board::_y(j);
    const piece_bitboard_t diags = M42::diag_attacks(i, occupied),
                           adiags = M42::adiag_attacks(i, occupied);
    const piece_bitboard_t bottomhalf = bitmask::full >> (board::LEN * (board::LEN - y_i));
    const piece_bitboard_t tophalf = bitmask::full << (board::LEN * (y_i + 1));
    piece_bitboard_t r = 0x00;
    if(x_j < x_i && y_i < y_j) {
      // top-left
      r = adiags & tophalf;
    } else if(x_i < x_j && y_i < y_j) {
      // top-right
      r = diags & tophalf;
    } else if(x_j < x_i && y_j < y_i) {
      // bottom-left
      r = diags & bottomhalf;
    } else if(x_i < x_j && y_j < y_i) {
      // bottom-right
      r = adiags & bottomhalf;
    }
    return (r & attacked_bit) ? r : 0x00;
  }

  inline piece_bitboard_t get_sliding_diag_attacking_xray(pos_t i, pos_t j, piece_bitboard_t friends, piece_bitboard_t foes) {
    const piece_bitboard_t blockers = get_sliding_diag_attack(i, friends|foes) & foes;
    return get_sliding_diag_attacking_ray(i, j, friends | (foes ^ blockers));
  }

  inline piece_bitboard_t get_sliding_diag_attacks(piece_bitboard_t mask, piece_bitboard_t occupied) {
    piece_bitboard_t ret = 0x00;
    bitmask::foreach(mask, [&](pos_t pos) mutable noexcept -> void {
      ret |= get_sliding_diag_attack(pos, occupied);
    });
    return ret;
  }


  inline piece_bitboard_t get_sliding_orth_attack(pos_t pos, piece_bitboard_t occupied) {
    return M42::rook_attacks(pos, occupied);
  }

  inline piece_bitboard_t get_sliding_orth_xray_attack(pos_t pos, piece_bitboard_t friends, piece_bitboard_t foes) {
    const piece_bitboard_t blockers = get_sliding_orth_attack(pos, friends|foes) & foes;
    return get_sliding_orth_attack(pos, friends | (foes ^ blockers));
  }

  inline piece_bitboard_t get_sliding_orth_attacking_ray(pos_t i, pos_t j, piece_bitboard_t occupied) {
    const piece_bitboard_t attacked_bit = piece::pos_mask(j);
    const pos_t x_i=board::_x(i), y_i=board::_y(i),
                x_j=board::_x(j), y_j=board::_y(j);
    if(x_i != x_j && y_i != y_j)return 0x00;
    piece_bitboard_t r = 0x00;;
    if(y_i == y_j) {
      const piece_bitboard_t rankattacks = M42_proxy::rank_attacks(i, occupied);
      const piece_bitboard_t shift = board::LEN * y_i;
      if(x_j < x_i) {
        const piece_bitboard_t left = (bitmask::hline >> (board::LEN - x_i)) << shift;
        r = rankattacks & left;
      } else if(x_i < x_j) {
        const piece_bitboard_t right = ((bitmask::hline << (x_i + 1)) & bitmask::hline) << shift;
        r = rankattacks & right;
      }
    } else if(x_i == x_j) {
      const piece_bitboard_t fileattacks = M42_proxy::file_attacks(i, occupied);
      if(y_i < y_j) {
        const piece_bitboard_t tophalf = bitmask::full << (board::LEN * (y_i + 1));
        r = fileattacks & tophalf;
      } else if(y_j < y_i) {
        const piece_bitboard_t bottomhalf = bitmask::full >> (board::LEN * (board::LEN - y_i));
        r = fileattacks & bottomhalf;
      }
    }
    return (r & attacked_bit) ? r : 0x00;
  }

  inline piece_bitboard_t get_sliding_orth_attacking_xray(pos_t i, pos_t j, piece_bitboard_t friends, piece_bitboard_t foes) {
    const piece_bitboard_t blockers = get_sliding_orth_attack(i, friends|foes) & foes;
    return get_sliding_orth_attacking_ray(i, j, friends | (foes ^ blockers));
  }

  inline piece_bitboard_t get_sliding_orth_attacks(piece_bitboard_t mask, piece_bitboard_t occupied) {
    piece_bitboard_t ret = 0x00;
    bitmask::foreach(mask, [&](pos_t pos) mutable noexcept -> void {
      ret |= get_sliding_orth_attack(pos, occupied);
    });
    return ret;
  }


  inline piece_bitboard_t get_knight_attack(pos_t pos) {
    return M42::knight_attacks(pos);
  }

  inline piece_bitboard_t get_knight_attacks(piece_bitboard_t mask) {
    return M42::calc_knight_attacks(mask);
  }


  constexpr pos_t uninitialized_king = 0xff;
  inline piece_bitboard_t get_king_attack(pos_t pos) {
    return M42::king_attacks(pos);
  }

  inline constexpr piece_bitboard_t get_king_castling_moves(COLOR c, pos_t i, piece_bitboard_t occupied, piece_bitboard_t attack_mask,
                                                              piece_bitboard_t castlings)
  {
    const pos_t shift = (c == WHITE) ? 0 : (board::SIZE-board::LEN);
    castlings &= bitmask::hline << shift;
    piece_bitboard_t castlemoves = 0x00;
    if(castlings) {
      // can't castle when checked
      if(attack_mask & piece::pos_mask(i))castlings=0x00;
      const piece_bitboard_t castleleft = 0x04ULL << shift;
      const piece_bitboard_t castleleftcheck = 0x0CULL << shift;
      const piece_bitboard_t castleleftcheckocc = 0x0EULL << shift;
      const piece_bitboard_t castleright = 0x40ULL << shift;
      const piece_bitboard_t castlerightcheck = 0x60ULL << shift;
      const piece_bitboard_t castlerightcheckocc = 0x60ULL << shift;
      if((castlings & castleleft)
          && !(attack_mask & castleleftcheck)
          && !(occupied & castleleftcheckocc))
        castlemoves|=castleleft;
      if((castlings & castleright)
          && !(attack_mask & castlerightcheck)
          && !(occupied & castlerightcheckocc))
        castlemoves|=castleright;
    }
    return castlemoves;
  }

  inline bool is_king_castling_move(COLOR c, pos_t i, pos_t j) {
    const pos_t shift = (c == WHITE) ? 0 : (board::SIZE-board::LEN);
    const piece_bitboard_t castlings = 0x44ULL << shift;
    const pos_t kingpos = board::_pos(E, 1) + shift;
    return (i == kingpos) && (piece::pos_mask(j) & castlings);
  }

  inline pos_pair_t get_king_castle_rook_move(COLOR c, pos_t i, pos_t j) {
    const pos_t castling_rank = (c == WHITE) ? 1 : 8;
    const pos_t castleleft = board::_pos(C, castling_rank);
    const pos_t castleright = board::_pos(G, castling_rank);
    if(j == castleleft) return bitmask::_pos_pair(board::_pos(A, castling_rank), board::_pos(D, castling_rank));
    if(j == castleright)return bitmask::_pos_pair(board::_pos(H, castling_rank), board::_pos(F, castling_rank));
    abort();
    return 0x00;
  }

  void print(piece_bitboard_t mask) {
    std::cout << piece::size(mask) << std::endl;
    bitmask::print(mask);
  }
} // namespace piece


// information about a piece kind
struct Piece {
  PIECE value;
  COLOR color;
  pos_t piece_index;

  static inline constexpr pos_t get_piece_index(PIECE p, COLOR c) {
    return (p==EMPTY) ? int(NO_PIECES)*int(NO_COLORS) : int(p)*(int)NO_COLORS+c;
  }

  constexpr inline Piece(PIECE p, COLOR c):
    value(p), color(c),
    piece_index(get_piece_index(p, c))
  {}

  inline constexpr char str() const {
    char c = '*';
    switch(value) {
      case EMPTY: return c;
      case PAWN: c = 'p'; break;
      case KNIGHT: c = 'n'; break;
      case BISHOP: c = 'b'; break;
      case ROOK: c = 'r'; break;
      case QUEEN: c = 'q'; break;
      case KING: c = 'k'; break;
    }
    return (color == WHITE) ? toupper(c) : c;
  }
};

// events : record what a move does

typedef uint_fast32_t event_t;

namespace event {
  constexpr event_t noevent = 0x00;
  constexpr event_t NULLMOVE_MARKER = 0xFF;

  constexpr pos_t marker_bits = 2;
  constexpr pos_t piece_ind_bits = 4;
  constexpr pos_t pos_bits = 6;
  constexpr pos_t move_bits = 14;
  constexpr pos_t move_bits_noprom = move_bits;
  constexpr pos_t killnothing = (1 << piece_ind_bits) - 1;
  constexpr pos_t enpassantnotrace = (1 << pos_bits) - 1;

  constexpr uint8_t BASIC_MARKER = 0x00;
  constexpr inline event_t basic(move_t m, pos_t killwhat, pos_t enpassant_trace=enpassantnotrace) {
    event_t e = 0x00;
    e = (e << pos_bits)  | enpassant_trace;
    e = (e << piece_ind_bits) | killwhat;
    e = (e << move_bits) | m;
    e = (e << marker_bits) | BASIC_MARKER;
    return e;
  }

  constexpr uint8_t CASTLING_MARKER = 0x01;
  constexpr inline event_t castling(move_t kingmove, move_t rookmove) {
    event_t e = 0x00;
    e = (e << move_bits) | rookmove;
    e = (e << move_bits) | kingmove;
    e = (e << marker_bits) | CASTLING_MARKER;
    return e;
  }

  constexpr uint8_t ENPASSANT_MARKER = 0x02;
  constexpr inline event_t enpassant(move_t m, pos_t killwhere) {
    event_t e = 0x00;
    e = (e << pos_bits)  | killwhere;
    e = (e << move_bits) | m;
    e = (e << marker_bits) | ENPASSANT_MARKER;
    return e;
  }

  constexpr uint8_t PROMOTION_MARKER = 0x03;
  // set j in the basic event has the promotion flag
  constexpr inline event_t promotion_from_basic(event_t basicevent) {
    return ((basicevent >> marker_bits) << marker_bits) | PROMOTION_MARKER;
  }

  inline uint8_t extract_marker(event_t &ev) {
    if(ev == event::noevent) {
      return NULLMOVE_MARKER;
    }
    pos_t byte = ev & ((1 << marker_bits) - 1);
    ev >>= marker_bits;
    return byte;
  }

  inline pos_t extract_byte(event_t &ev) {
    pos_t byte = ev & 0xFF;
    ev >>= 8;
    return byte;
  }

  inline pos_t extract_piece_ind(event_t &ev) {
    pos_t p = ev & ((1 << piece_ind_bits) - 1);
    ev >>= piece_ind_bits;
    return p;
  }

  inline pos_t extract_pos(event_t &ev) {
    pos_t p = ev & ((1 << pos_bits) - 1);
    ev >>= 6;
    return p;
  }

  inline move_t extract_move(event_t &ev) {
    move_t m = ev & ((1ULL << move_bits) - 1);
    ev >>= move_bits;
    return m;
  }

  inline move_t extract_move_noprom(event_t &ev) {
    move_t m = ev & ((1ULL << move_bits_noprom) - 1);
    ev >>= move_bits_noprom;
    return m;
  }
} // namespace event


namespace fen {
  typedef struct _FEN {
    std::string board;
    COLOR active_player : 2;
    pos_t castling_compressed : 4;
    pos_t enpassant;
    pos_t halfmove_clock;
    uint16_t fullmove;

    inline bool operator==(const struct _FEN &other) const {
      return board == other.board && active_player == other.active_player &&
             castling_compressed == other.castling_compressed &&
             enpassant == other.enpassant && halfmove_clock == other.halfmove_clock
             && fullmove == other.fullmove;
    }

    inline bool operator!=(const struct _FEN &other) const {
      return !operator==(other);
    }
  } FEN;

  constexpr inline pos_t compress_castlings(piece_bitboard_t castlings) {
    pos_t comp = 0;
    comp |= (castlings & 0x04LLU) ? 1 : 0; comp <<= 1;
    comp |= (castlings & 0x40LLU) ? 1 : 0; comp <<= 1;
    comp |= (castlings & (0x04LLU << (board::SIZE-board::LEN))) ? 1 : 0; comp <<= 1;
    comp |= (castlings & (0x40LLU << (board::SIZE-board::LEN))) ? 1 : 0;
    return comp;
  }

  constexpr inline piece_bitboard_t decompress_castlings(pos_t comp) {
    piece_bitboard_t castlings = 0x00;
    if(comp&1)castlings|=(0x40LLU << (board::SIZE-board::LEN));
    comp>>=1;
    if(comp&1)castlings|=(0x04LLU << (board::SIZE-board::LEN));
    comp>>=1;
    if(comp&1)castlings|=(0x40LLU);
    comp>>=1;
    if(comp&1)castlings|=(0x04LLU);
    comp>>=1;
    return castlings;
  }

  FEN load_from_string(std::string s) {
    size_t i = 0;
    FEN f = {
      .board = std::string(),
      .active_player = WHITE,
      .castling_compressed = 0x00,
      .enpassant = event::enpassantnotrace,
      .halfmove_clock = 0,
      .fullmove = 0,
    };
    // board
    for(const char *c = s.c_str(); *c != '\0' && *c != ' '; ++c, ++i) {
      if(*c == '/')continue;
      if(isdigit(*c)) {
        for(int j=0;j<*c-'0';++j)f.board+=' ';
      } else {
        f.board += *c;
      }
    }
    assert(f.board.length() <= board::SIZE);
    // skip space
    while(isspace(s[i]))++i;
    // active plaer
    assert(index("wWbB", s[i]) != nullptr);
    if(s[i] == 'b' || s[i] == 'B')f.active_player=BLACK;
    ++i;
    // skip space
    while(isspace(s[i]))++i;
    // castlings
    piece_bitboard_t castlings = 0x00;
    while(!isspace(s[i])) {
      assert(index("KkQqAHah-", s[i]) != nullptr);
      pos_t blackline = board::SIZE-board::LEN;
      switch(s[i]) {
        case 'K':case 'H': castlings|=0x40ULL; break;
        case 'Q':case 'A': castlings|=0x04ULL; break;
        case 'k':case 'h': castlings|=0x40ULL<<blackline; break;
        case 'q':case 'a': castlings|=0x04ULL<<blackline; break;
        case '-':break;
      }
      ++i;
    }
    f.castling_compressed = fen::compress_castlings(castlings);
    // skip space
    while(isspace(s[i]))++i;
    // enpassant
    char probe = s[i];
    if(probe != '-') {
      char x = probe;
      assert(index("abcdefgh", x)!=nullptr);
      x -= 'a';
      char y = s[++i];
      assert(index("12345678", y)!=nullptr);
      y -= '1';
      f.enpassant = board::_pos(A+x, 1+y);
    } else f.enpassant = event::enpassantnotrace;
    // skip space
    while(isspace(s[i]))++i;
    // half-moves
    int sc;
    sc = sscanf(&s[i], "%hhu", &f.halfmove_clock);
    assert(sc != -1);
    i += sc;
    // skip space
    while(isspace(s[i]))++i;
    // fullmoves
    // TODO fix
    sc = sscanf(&s[i], "%hu", &f.fullmove);
    assert(sc != -1);
    i += sc;
    // done
    return f;
  }

  FEN load_from_file(std::string fname) {
    FILE *fp = fopen(fname.c_str(), "r");
    assert(fp != nullptr);
    std::string s; char c;
    while((c=fgetc(fp))!=EOF)s+=c;
    fclose(fp);
    return load_from_string(s);;
  }

  const FEN starting_pos = fen::load_from_string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"s);
  const FEN search_explosion_pos = fen::load_from_string("q2k2q1/2nqn2b/1n1P1n1b/2rnr2Q/1NQ1QN1Q/3Q3B/2RQR2B/Q2K2Q1 w - -"s);

  FEN export_from_board(const Perft &board);

  std::string export_as_string(const fen::FEN &f) {
    std::string s = ""s;
    for(pos_t y_ = 0; y_ < board::LEN; ++y_) {
      const pos_t y = board::LEN - y_ - 1;
      pos_t emptycount = 0;
      for(pos_t x = 0; x < board::LEN; ++x) {
        const pos_t ind = board::_pos(A+x, 1+y);
        if(f.board[ind] == ' ') {
          ++emptycount;
          continue;
        }
        if(emptycount > 0) {
          s += std::to_string(emptycount);
          emptycount = 0;
        }
        s += f.board[ind];
      }
      if(emptycount > 0) {
        s += std::to_string(emptycount);
        emptycount = 0;
      }
      if(y != 0)s+="/";
    }
    s += ' ';
    s += (f.active_player == WHITE) ? 'w' : 'b';
    s += ' ';
    if(!f.castling_compressed) {
      s += '-';
    } else {
      if(f.castling_compressed & (0x1 << 3))s+='K';
      if(f.castling_compressed & (0x1 << 2))s+='Q';
      if(f.castling_compressed & (0x1 << 1))s+='k';
      if(f.castling_compressed & (0x1 << 0))s+='q';
    }
    s += ' ';
    s += (f.enpassant == event::enpassantnotrace) ? "-"s : board::_pos_str(f.enpassant);
    s += " "s + std::to_string(f.halfmove_clock) + " "s + std::to_string(f.fullmove);
    return s;
  }

  std::string export_as_string(const Perft &board) {
    std::string s = export_as_string(export_from_board(board));
    assert(s == export_as_string(fen::load_from_string(s)));
    return s;
  }
} // namespace fen


#ifndef ZOBRIST_SIZE
#define ZOBRIST_SIZE (1ULL << 22)
#endif

namespace zobrist {

using key_t = uint32_t;
using ind_t = uint16_t;

constexpr ind_t rnd_start_piecepos = 0;
constexpr ind_t rnd_start_enpassant = rnd_start_piecepos + board::SIZE * (board::NO_PIECE_INDICES - 1);
constexpr ind_t rnd_start_castlings = rnd_start_enpassant + 8;
constexpr ind_t rnd_start_moveside = rnd_start_castlings + 4;
constexpr ind_t rnd_size = rnd_start_moveside + 1;
std::array<key_t, rnd_size> rnd_hashes;

// http://vigna.di.unimi.it/ftp/papers/xorshift.pdf
uint64_t seed = 0x78473ULL;

inline void set_seed(uint64_t new_seed) {
  seed = new_seed;
}

uint64_t randint() {
  seed ^= seed >> 12;
  seed ^= seed << 25;
  seed ^= seed >> 27;
  return seed * 2685821657736338717ULL;
}

void init() {
  static_assert(rnd_start_moveside + 1 < ZOBRIST_SIZE);
  std::unordered_set<key_t> rnd_seen;
  for(ind_t i = 0; i < rnd_hashes.size(); ++i) {
    key_t r;
    do {
      r = randint() % ZOBRIST_SIZE;
      rnd_hashes[i] = r;
    } while(rnd_seen.find(r) != rnd_seen.end());
    rnd_seen.insert(r);
  }
}


template <typename T> using ttable = std::array<T, ZOBRIST_SIZE>;
template <typename T> using ttable_ptr = ttable<T> *;

template<typename InnerObject>
struct StoreScope {
  ttable_ptr<InnerObject> &zb_store;
  bool is_outer_scope;

  explicit inline StoreScope(ttable_ptr<InnerObject> &scope_ptr):
    zb_store(scope_ptr),
    is_outer_scope(scope_ptr == nullptr)
  {
    if(zb_store == nullptr) {
      zb_store = new ttable<InnerObject>{};
      reset();
    }
  }

  void reset() {
    for(size_t i = 0; i < ZOBRIST_SIZE; ++i) {
      zb_store->at(i).info.unset();
    }
  }

  ttable<InnerObject> &get_object() {
    return *zb_store;
  }

  void end_scope() {
    if(is_outer_scope) {
      delete zb_store;
      zb_store = nullptr;
      is_outer_scope = false;
    }
  }

  inline ~StoreScope() {
    end_scope();
  }
};

template <typename InnerObject>
decltype(auto) make_store_object_scope(ttable_ptr<InnerObject> &zb_store) {
  return StoreScope<InnerObject>(zb_store);
}

} // zobrist
