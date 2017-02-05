#ifndef __BOARD_H__
#define __BOARD_H__

#include "common.hpp"

using namespace std;

struct BoardHalf {
    uint64_t squares;

    BoardHalf() : squares(0) { }
    BoardHalf(uint64_t squares) : squares(squares) { }

    inline void clear() {
        squares = 0;
    }

    inline bool get(size_t i) const {
        return (squares >> i) & 1;
    }

    inline void set(size_t i, bool v) {
        squares = (squares & ~((uint64_t)1 << i)) | ((uint64_t)v << i);
    }

    inline bool move(size_t i) {
        return !get(i) && (set(i, true), true);
    }

    inline int8_t count() const {
#if (defined(__GNUC__) || defined(__clang__))
        return __builtin_popcount(squares);
#else
        int8_t result = 0;
        for (size_t i = 0; i < 64; i++)
            result += operator[](i);
        return result;
#endif
    }
};

struct Board {
    static Board standard();

    BoardHalf black;
    BoardHalf white;

    Board() = default;
    Board(BoardHalf black, BoardHalf white) : black(black), white(white) { }

    inline bool occupied(size_t i) {
        return black.get(i) && white.get(i);
    }

    inline bool move(Side s, Move m) {
        return (s == Side::WHITE ? white : black).move(m.index());
    }

    void setBoard(char data[]);
};

#endif
