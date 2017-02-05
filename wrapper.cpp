#include <iostream>

#include <cstdlib>
#include <cstring>

#include "player.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " SIDE" << std::endl;
        return -1;
    }

    Side side = (!strcmp(argv[1], "Black")) ? Side::BLACK : Side::WHITE;
    Side opponent = (!strcmp(argv[1], "Black")) ? Side::WHITE : Side::BLACK;
    Board board = Board::standard();
    Player player(side);

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    // Tell java wrapper that we are done initializing.
    std::cout << "Init done" << std::endl;
    std::cout.flush();


    // Get opponent's move and time left for player each turn.
    int x, y, remaining;
    while (cin >> x >> y >> remaining) {
        if (x >= 0 && y >= 0)
            board.move(opponent, Move(x, y));

        // Get player's move and output to java wrapper.
        Move move = player.move(board, remaining);
        cout << move.x << ' ' << move.y << endl;
        cout.flush();
        cerr.flush();
    }

    return 0;
}
