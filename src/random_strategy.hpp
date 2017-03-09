#ifndef __RANDOM_STRATEGY_HPP__
#define __RANDOM_STRATEGY_HPP__

#include "board.hpp"
#include "common.hpp"

struct random_strategy {
    Move move(Board board, int remaining);
};

#endif
