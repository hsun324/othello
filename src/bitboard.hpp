#ifndef __BITBOARD_HPP__
#define __BITBOARD_HPP__

#include <cstdint>
#include <ostream>

struct Bitboard {
    uint64_t board;

    Bitboard() : board(0) { }
    Bitboard(uint64_t board) : board(board) { }

    inline uint64_t raw() const {
        return board;
    }

    inline void reset(uint64_t _board = 0ULL) {
        board = _board;
    }

    inline bool get(uint8_t i) const {
        return (board >> i) & ((uint64_t) 1);
    }

    inline void set(uint8_t i) {
        board |= ((uint64_t) 1 << i);
    }
    inline void unset(uint8_t i) {
        board &= ~((uint64_t) 1 << i);
    }

    inline void put(uint8_t i, bool v) {
        board &= ~((uint64_t) 1 << i);
        board |= ((uint64_t) v << i);
    }

    inline int8_t count() const {
#if (defined(__GNUC__) || defined(__clang__))
        return __builtin_popcount(board);
#else
        int8_t result = 0;
        for (size_t i = 0; i < 64; i++)
            result += get(i);
        return result;
#endif
    }

    inline Bitboard dilate() const {
        uint64_t sl = (board << 1) && 0xFEFEFEFEFEFEFEFEULL;
        uint64_t sr = (board >> 1) && 0x7F7F7F7F7F7F7F7FULL;

        return Bitboard(
            board | (board << 8) | (board >> 8) | sl | sr |
            (sr << 8) | (sr >> 8) | (sl << 8) | (sl >> 8)
        );
    }
};

inline Bitboard operator|(const Bitboard r, const Bitboard l) {
    return Bitboard(r.raw() | l.raw());
}

inline Bitboard operator&(const Bitboard r, const Bitboard l) {
    return Bitboard(r.raw() & l.raw());
}

inline Bitboard operator~(const Bitboard r) {
    return Bitboard(~r.raw());
}

inline bool operator==(const Bitboard r, const Bitboard l) {
    return r.raw() == l.raw();
}


inline std::ostream &operator<<(std::ostream &os, const Bitboard board) {
    for (int i = 0; i < 64; i += 8) {
        for (int j = i; j < i + 8; j++) {
            os << (board.get(j) ? '1' : '.');
        }
        os << std::endl;
    }
    return os;
}

#endif
