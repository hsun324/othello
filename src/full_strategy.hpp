#ifndef __FULL_STRATEGY_HPP__
#define __FULL_STRATEGY_HPP__

#include "board.hpp"
#include "common.hpp"

struct full_strategy {
    Move move(Board board, int remaining);
};

#endif
