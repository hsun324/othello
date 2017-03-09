#ifndef __MINIMAX_STRATEGY_HPP__
#define __MINIMAX_STRATEGY_HPP__

#include "board.hpp"
#include "common.hpp"

struct minimax_strategy {
    Move move(Board board, int remaining);
};

#endif
