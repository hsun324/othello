#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "board.hpp"

template<typename Strategy>
struct Player {
    Board board;
    Strategy strategy;

    Player(Board board) : board(board), strategy() { }

    bool move(Side s, Move m) {
        return board.move(s, m.index());
    }
    Move next_move(int remaining) {
        Move m = strategy.move(board, remaining);

        move(Side::BLACK, m);

        return m;
    }

    Move *doMove(int remaining) {
        Move m = next_move(remaining);

        if (m.x < 0 && m.y < 0)
            return nullptr;

        return new Move(m);
    }
};

#endif
