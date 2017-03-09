#ifndef __BOARD_H__
#define __BOARD_H__

#include "bitboard.hpp"
#include "common.hpp"

struct MoveBoard {
    Bitboard total;
    Bitboard directional[8];
};

struct Board {
    static Board standard();

    Bitboard black;
    Bitboard white;

    Board() : black(), white() { };
    Board(uint64_t black, uint64_t white)
        : black(black), white(white) { };

    inline void reset(uint64_t _black, uint64_t _white) {
        black.reset(_black);
        white.reset(_white);
    }

    inline bool occupied(int i) const {
        return black.get(i) && white.get(i);
    }

    inline Bitboard occupied() const {
        return Bitboard(black.raw() | white.raw());
    }
    inline Bitboard unoccupied() const {
        return Bitboard(~(black.raw() | white.raw()));
    }

    MoveBoard valid_moves() const;

    bool move(Side s, int i);

    void setBoard(char data[]);

    inline Board inverse() const {
        return Board(white.raw(), black.raw());
    }
};

inline std::ostream& operator<<(std::ostream &os, const Board b) {
    for (int i = 0; i < 64; i += 8) {
        for (int j = i; j < i + 8; j++) {
            os << (b.black.get(j) ? 'b' : (b.white.get(j) ? 'w' : '.'));
        }
        os << std::endl;
    }
    return os;
}

#endif
