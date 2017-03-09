#ifndef __COMMON_H__
#define __COMMON_H__

#include <cstddef>
#include <cstdint>

#include <iostream>

enum class Side {
    WHITE, BLACK
};

struct Move {
    int x, y;

    Move() : x(0), y(0) { }
    Move(int i) : x(i % 8), y(i / 8) { }
    Move(int x, int y) : x(x), y(y) { }

    inline int index() const {
        return y * 8 + x;
    }
};

#endif
