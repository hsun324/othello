#include "board.hpp"

Board Board::standard() {
    return Board(0x00000010080000000ULL, 0x0000000810000000ULL);
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    black.clear();
    white.clear();

    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            black.move(i);
        } if (data[i] == 'w') {
            white.move(i);
        }
    }
}
