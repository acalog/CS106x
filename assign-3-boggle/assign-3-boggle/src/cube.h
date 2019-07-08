/**
  File: cube.h
  ------------
  Defines a cube type to represent boggle blocks
  on the game board.
  */

#ifndef CUBE_H
#define CUBE_H

#include <iostream>
using namespace std;

/**
  Defines positive integer grid coordinates for
  location of block on boggle board. Also holds value of
  block, a single alpha-char.
  */

struct block {
    int row;
    int col;
    char letter;
};

inline bool operator<(const block& one, const block& two) {
    return one.row < two.row || (one.row == two.row && one.col < two.col);
}

#endif // CUBE_H
