# M42
C++ Library for Bitboard Attack Mask Generation

**Introduction**
---
M42 is a small library for Bitboard Attack Mask Generation. It is intended to be used by beginner/novice chess programmers. The purpose of this library is to provide the fastest attack mask generation methods available to chess programming amateurs, and to a save time and energy on coding and understanding those methods.

**Getting Started**
---
Using M42 library is as simple as it can be, just include the header file (#include “m42.h”) and
compile “m42.cpp” along with the source files of your program. No DLLs, no fancy precompiled
libraries, no special command lines. You also need to call M42::init() in the main function of
your program.

**SUPPORTED COMPILERS**
---
M42 is intended to be used under a variety of C++ compilers on a variety of system. M42 has been
tested on g++ and Visual C++ compilers. M42 should compile fine under pretty much every C++
compiler, and if it doesn’t, you can of course email your issues (see the cover page).

**Quickstart - see example.cpp**
---
A file by the name “example.cpp” is included to demonstrate the usage and some functions of M42
library. You don’t need to set any special command line options to compile it. Just compile it like
any other source file. For example, under g++, use the command line “g++ m42.cpp example.cpp –o
m42” and then run the compiled program by “./m42”.

# Functions provided by M42 library

The M42 library provides a handful of functions for attack mask generation of all standard chess pieces. *Make sure to call ```M42::init()``` before using these functions.*

***SLIDER ATTACK MASK GENERATION FUNCTIONS***
---

For rooks, bishops, and queens, use the functions M42::rook_attacks(<square>,
<occupancy>), M42::bishop_attacks(<square>, <occupancy>),
M42::queen_attacks(<square>, <occupancy>), respectively. These functions take the square
of the piece as their first argument (a1 = 0, a2 = 1, …, h8 = 63), and the occupancy bitboard as their
second argument. Note that if the piece itself is included in the occupancy bitboard, it would cause
NO problem. See the function reference table below for complete list of functions.
  
***NON-SLIDER ATTACK MASK GENERATION FUNCTIONS***
---
In this category, M42 provides functions for handling single pieces as well as multiple pieces.
Function that work for multiple pieces are particularly useful in evaluation.

***Pawns***
---
For pawn attacks, use the function M42::pawn_attacks(<color of pawn>, <square of
pawn>). The first argument, the color of pawn, must be 0 if the color is white, or 1 if the color is
black. You also have the template function M42::calc_pawn_attacks(<pawns bitboard>)
which takes the color as template argument, and the pawns bitboard which may contain multiple
pawns.

***King and Knight***
---
M42 provides functions M42::king_attacks(<square of king>) and
M42::knight_attacks(<square of knight>) for attack mask generation of kings and knights
respectively, given the square of the piece. You also have M42::calc_king_attacks(<bitboard
of kings>) and M42::calc_knight_attacks(<bitboard of knights>) for attack mask
generation of multiple kings and knights, particularly useful in evaluation of attacks. These
functions, although slower than their single-piece attack mask generator brothers, are still very fast.

# Reference
See (M42.pdf)[https://sites.google.com/site/sydfhd/projects/m42/M42.pdf?attredirects=0&d=1].
