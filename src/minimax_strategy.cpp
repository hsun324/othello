#include "minimax_strategy.hpp"

int value(Board board) {
    return board.black.count() - board.white.count();
}

struct SearchReturn {
    int value;
    int move;
};

SearchReturn search(Board board, int depth, int mult) {
    if (depth == 0) return { value(board) * mult, -9 };

    MoveBoard moveboard = board.valid_moves();

    if (moveboard.total.raw() == 0) return { value(board) * mult, -9 };

    int max = -1000000000;
    int max_i = -9;

    for (int i = 0; i < 64; i++) {
        if (moveboard.total.get(i)) {
            Board next = Board(board);

            if (mult > 0) next.move(Side::BLACK, i);
            else next.move(Side::WHITE, i);

            int val = -search(next, depth - 1, -mult).value;

            // std::cerr << "searching @ " << depth << " " << i << " - " << val << std::endl;

            if (val > max) {
                max = val;
                max_i = i;
            }
        }
    }

    return { max, max_i };
}

Move minimax_strategy::move(Board board, int remaining) {
    int move = search(board, 2, 1).move;
    return Move(move);
    
}

