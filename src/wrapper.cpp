#include <iostream>

#include <cstdlib>
#include <cstring>

#include "full_strategy.hpp"

#define STRATEGY full_strategy

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " SIDE" << std::endl;
        return -1;
    }

    Board board = Board::standard();
    STRATEGY strategy;

    if (!strcmp(argv[1], "White")) {
        board = board.flip();
    } else if (strcmp(argv[1], "Black")) {
        std::cerr << "SIDE can only be Black or White" << std::endl;
        return -2;
    }

    // let tournament know we are ready
    std::cout << "Init done" << std::endl;

    int x, y, remaining;
    while (std::cin >> x >> y >> remaining) {
        if (x >= 0 && y >= 0)
            board = board.flip().move(Move(x, y).index()).flip();

        // Get player's move and output to java wrapper.
        Move move = strategy.move(board, remaining);
        board = board.move(move.index());

        std::cout << move.x << ' ' << move.y << std::endl;
    }

    return 0;
}
