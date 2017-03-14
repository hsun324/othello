#include "thirdparty/spookyhash.hpp"

#include "full_strategy.hpp"

#include <cmath>
#include <cstring>
#include <unistd.h>

using namespace std;

namespace full {
    struct SearchReturn {
        int value;
        int move;
    };

    struct TableEntry {
        Board board;
        int value;
    };

    const int piece_table[64] = {
        115, 15, 5, 5, 5, 5, 15, 115,
        15,   1, 1, 1, 1, 1,  1,  15,
        5,    1, 1, 1, 1, 1,  1,   5,
        5,    1, 1, 1, 1, 1,  1,   5,
        5,    1, 1, 1, 1, 1,  1,   5,
        5,    1, 1, 1, 1, 1,  1,   5,
        15,   1, 1, 1, 1, 1,  1,  15,
        115, 15, 5, 5, 5, 5, 15, 115
    };

    const int move_order[64] = {
        0, 7, 56, 63,
        1, 6, 8, 15, 48, 55, 57, 62,
        2, 3, 4, 5,
        9, 10, 11, 12, 13, 14,
        16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31,
        32, 33, 34, 35, 36, 37, 38, 39,
        40, 41, 42, 43, 44, 45, 46, 47,
        49, 50, 51, 52, 53, 54,
        58, 59, 60, 61
    };

    TableEntry table[1 << 20];

    inline int value(Board board, MoveBoard smoves, MoveBoard omoves) {
        int occupancy = board.occupied().count();

        int spieces = board.self.count();
        int opieces = board.opponent.count();

        int smobility = smoves.total.count();
        int omobility = omoves.total.count();

        if (occupancy >= 64 || smobility + omobility <= 0)
            return 20000 * (spieces > opieces) - 10000;

        int parity = 0;
        if (occupancy > 48) parity = 100 * (occupancy - 48) / 16 * (spieces - opieces) / (spieces + opieces); 

        int mobility = 0;
        if (smobility + omobility > 0) mobility = 50 * (occupancy / 22 + 1) * (smobility - omobility) / (smobility + omobility);

        int position = 0;
        for (int i = 0; i < 64; i++) {
             if (board.self.get(i)) position += 2 * piece_table[i];
             if (board.opponent.get(i)) position -= 2 * piece_table[i];
             if (smoves.total.get(i)) position += piece_table[i];
             if (omoves.total.get(i)) position -= piece_table[i];
        }

#if FULLDEBUG
        std::cerr << smoves.total << std::endl;
        std::cerr << omoves.total << std::endl;
        std::cerr << "M " << mobility << " P " << position << std::endl;
#endif

        return parity + mobility + position;
    }

    SearchReturn search(Board board, int depth, int a, int b) {
        MoveBoard smoves = board.moves();

        if (depth == 0 || smoves.total.raw() == 0)
            return { value(board, smoves, board.flip().moves()), -9 };

        int max = -10000000;
        int max_i = -9;

        for (int c = 0; c < 64; c++) {
            int i = move_order[c];
            if (smoves.total.get(i)) {
                Board next = board.move(i);

                struct { uint64_t a, b; } hashable = { next.self.raw(), next.opponent.raw() };
                int hash = SpookyHash::Hash64(&hashable, sizeof(hashable), 0) % (1 << 20);

                int val = 0;
                if (table[hash].board == next) {
                    val = table[hash].value;
                } else {
                    val = search(next.flip(), depth - 1, b, a).value;
                    table[hash].board = next;
                    table[hash].value = val;
                }

                if (val > max) {
                    max = val;
                    max_i = i;
                }

                if (val > a) {
                    a = val;
                }

                if (a >= b) break;
            }
        }

        return { max, max_i };
    }
}

Move full_strategy::move(Board board, int remaining) {
    int depth = 10;
    if (board.occupied().count() > 28) depth = 16;
    if (board.occupied().count() > 32) depth = 22;
    if (board.occupied().count() > 36) depth = 26;

    memset(&full::table, 0, sizeof(full::table));
    int move = full::search(board, depth, -10000000, 10000000).move;

    return Move(move);
}

