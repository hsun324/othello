#ifndef __BOARD_H__
#define __BOARD_H__

#include "bitboard.hpp"
#include "common.hpp"

struct MoveBoard {
    Bitboard total;
    Bitboard directional[8];
};

struct Board {
    static Board standard() {
        return Board(0x0000000810000000ULL,
                     0x0000001008000000ULL);
    }


    Bitboard self;
    Bitboard opponent;

    Board() : self(), opponent() { };
    Board(Bitboard self, Bitboard opponent)
        : self(self), opponent(opponent) { };

    inline bool occupied(int i) const {
        return occupied().get(i);
    }

    inline Bitboard occupied() const {
        return self | opponent;
    }
    inline Bitboard unoccupied() const {
        return ~occupied();
    }

    MoveBoard moves() const;
    Board move(int i);

    inline Board flip() const {
        return Board(opponent, self);
    }
};

inline bool operator==(const Board l, const Board r) {
    return l.self == r.self && l.opponent == r.opponent;
}

inline std::ostream& operator<<(std::ostream &os, const Board b) {
    for (int i = 0; i < 64; i += 8) {
        for (int j = i; j < i + 8; j++) {
            os << (b.self.get(j) ? 'b' : (b.opponent.get(j) ? 'w' : '.'));
        }
        os << std::endl;
    }
    return os;
}

#endif
