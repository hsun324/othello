#include "full_strategy.hpp"

#include <cmath>
#include <unistd.h>

using namespace std;

namespace full {
    struct SearchReturn {
        int value;
        int move;
    };

    const int piece_table[64] = {
        200, -50, 0, 0, 0, 0, -50, 200,
        -50, -20, 0, 0, 0, 0, -20, -50,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        -50, -20, 0, 0, 0, 0, -20, -50,
        200, -50, 0, 0, 0, 0, -50, 200,
    };

    inline int value(Board board, MoveBoard smoves, MoveBoard omoves) {
        // int ssqrcnt = board.black.count();
        // int osqrcnt = board.white.count();

        int smobcnt = smoves.total.count();
        int omobcnt = omoves.total.count();

        int mobscore = 0;
        if (smobcnt + omobcnt > 0) mobscore = 100 * (smobcnt - omobcnt) / (smobcnt + omobcnt);

        int spotmob = Bitboard(board.unoccupied().raw() & board.white.dilate().raw()).count();
        int opotmob = Bitboard(board.unoccupied().raw() & board.black.dilate().raw()).count();

        //std::cerr << spotmob << ", " << opotmob << std::endl;

        int potscore = 0;
        if (spotmob + opotmob > 0) potscore = 100 * (spotmob - opotmob) / (spotmob + opotmob);

        int sqrscore = 0;

        for (int i = 0; i < 64; i++) {
             if (board.black.get(i)) sqrscore += piece_table[i];
             if (board.white.get(i)) sqrscore -= piece_table[i];
        }

        return mobscore + potscore + sqrscore;
    }

    SearchReturn search(Board board, int depth, int mult) {
        MoveBoard smoves = board.valid_moves();

        if (depth == 0 || smoves.total.raw() == 0)  {
            MoveBoard omoves = board.inverse().valid_moves();

            return { value(board, smoves, omoves) * mult, -9 };
        }

        int max = -1000000000;
        int max_i = -9;

        uint64_t smove_raw = smoves.total.raw();
        for (int i = 0; smove_raw != 0; i++, smove_raw >>= 1) {
            if (smove_raw & 1) {
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
}

Move full_strategy::move(Board board, int remaining) {
    int move = full::search(board, 6, 1).move;

    return Move(move);
}

