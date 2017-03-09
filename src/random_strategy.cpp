#include "random_strategy.hpp"

#include <iomanip>
#include <iostream>

Move random_strategy::move(Board board, int remaining) {
    MoveBoard moveboard = board.valid_moves();

    Move moves[64];
    int move_count = 0;
    
    for (int i = 0; i < 64; i++)
        if (moveboard.total.get(i))
            moves[move_count++] = Move(i);

    if (move_count == 0) return Move(-1, -1);
    else return moves[0];
}
