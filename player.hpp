#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "common.hpp"
#include "board.hpp"

class Player {

public:
    Player(Side side);
    ~Player();

    Move move(Board board, int remaining);
    Move *doMove(Board board, int remaining);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
