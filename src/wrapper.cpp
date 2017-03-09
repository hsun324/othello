#include <iostream>

#include <cstdlib>
#include <cstring>

#include "player.hpp"
#include "full_strategy.hpp"

#define BLACK_STARTING 0x0000000810000000ULL
#define WHITE_STARTING 0x0000001008000000ULL

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " SIDE" << std::endl;
        return -1;
    }

    Board board(BLACK_STARTING, WHITE_STARTING);

    if (!strcmp(argv[1], "White")) {
        board = board.inverse();
    } else if (strcmp(argv[1], "Black")) {
        std::cerr << "side must be one of Black or White" << std::endl;
        return -2;
    }

    Player<full_strategy> player(board);

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    // Tell java wrapper that we are done initializing.
    std::cout << "Init done" << std::endl;
    std::cout.flush();


    // Get opponent's move and time left for player each turn.
    int x, y, remaining;
    while (std::cin >> x >> y >> remaining) {
        if (x >= 0 && y >= 0)
            player.move(Side::WHITE, Move(x, y).index());

        // Get player's move and output to java wrapper.
        Move move = player.next_move(remaining);
        std::cout << move.x << ' ' << move.y << std::endl;
        std::cout.flush();
        std::cerr.flush();
    }

    return 0;
}
