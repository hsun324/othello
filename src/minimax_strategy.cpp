#include "minimax_strategy.hpp"

int value(Board board) {
    return board.self.count() - board.opponent.count();
}

struct SearchReturn {
    int value;
    int move;
};

SearchReturn search(Board board, int depth) {
    MoveBoard moveboard = board.moves();

    if (depth == 0 || moveboard.total.raw() == 0)
        return { value(board), -9 };

    int max = -1000000000;
    int max_i = -9;

    for (int i = 0; i < 64; i++) {
        if (moveboard.total.get(i)) {
            int val = search(board.move(i).flip(), depth - 1).value;
            if (val > max) {
                max = val;
                max_i = i;
            }
        }
    }

    return { max, max_i };
}

Move minimax_strategy::move(Board board, int remaining) {
    int move = search(board, 2).move;
    return Move(move);
    
}

