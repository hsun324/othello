#ifndef __COMMON_H__
#define __COMMON_H__

#include <cstddef>
#include <cstdint>

enum class Side {
    WHITE, BLACK
};

struct Move {
    int32_t x, y;

    Move(int32_t x, int32_t y) : x(x), y(y) { }

    inline size_t index() const {
        return (((uint8_t) x) << 3) + ((uint8_t) y);
    }
};

#endif
