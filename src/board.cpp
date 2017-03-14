#include "board.hpp"

#include <bitset>
#include <iomanip>

#define MOVE_U_MASK (0x00FFFFFFFFFFFF00ULL)
#define MOVE_D_MASK (0x00FFFFFFFFFFFF00ULL)
#define MOVE_R_MASK (0x7E7E7E7E7E7E7E7EULL)
#define MOVE_L_MASK (0x7E7E7E7E7E7E7E7EULL)

#define MOVE_UL_MASK (MOVE_U_MASK & MOVE_L_MASK)
#define MOVE_UR_MASK (MOVE_U_MASK & MOVE_R_MASK)
#define MOVE_DL_MASK (MOVE_D_MASK & MOVE_L_MASK)
#define MOVE_DR_MASK (MOVE_D_MASK & MOVE_R_MASK)

#define MOVE_U_SHIFT(X) ((X) >> 8)
#define MOVE_D_SHIFT(X) ((X) << 8)
#define MOVE_L_SHIFT(X) ((X) >> 1)
#define MOVE_R_SHIFT(X) ((X) << 1)

#define MOVE_UL_SHIFT(X) ((X) >> 9)
#define MOVE_UR_SHIFT(X) ((X) >> 7)
#define MOVE_DL_SHIFT(X) ((X) << 7)
#define MOVE_DR_SHIFT(X) ((X) << 9)

#define MOVE_MASK(MOVE, VAL) ((VAL) & MOVE_##MOVE##_MASK)
#define MOVE_SHIFT(MOVE, VAL) MOVE_##MOVE##_SHIFT(VAL)

#define MOVE_MS(DEST, MOVE, SELF, OPP) do { \
    uint64_t mask = MOVE_MASK(MOVE, OPP); \
    uint64_t temp = mask & MOVE_SHIFT(MOVE, SELF); \
    temp |= mask & MOVE_SHIFT(MOVE, temp); \
    temp |= mask & MOVE_SHIFT(MOVE, temp); \
    temp |= mask & MOVE_SHIFT(MOVE, temp); \
    temp |= mask & MOVE_SHIFT(MOVE, temp); \
    temp |= mask & MOVE_SHIFT(MOVE, temp); \
    DEST = (~((SELF) | (OPP))) & (MOVE_SHIFT(MOVE, temp)); \
} while (0)


MoveBoard Board::moves() const {
    MoveBoard moves;

    uint64_t sel = self.raw(), opp = opponent.raw();

    MOVE_MS(moves.directional[0], U,  sel, opp);
    MOVE_MS(moves.directional[1], D,  sel, opp);
    MOVE_MS(moves.directional[2], L,  sel, opp);
    MOVE_MS(moves.directional[3], R,  sel, opp);
    MOVE_MS(moves.directional[4], UL, sel, opp);
    MOVE_MS(moves.directional[5], UR, sel, opp);
    MOVE_MS(moves.directional[6], DL, sel, opp);
    MOVE_MS(moves.directional[7], DR, sel, opp);

    moves.total =
        moves.directional[0] |
        moves.directional[1] |
        moves.directional[2] |
        moves.directional[3] |
        moves.directional[4] |
        moves.directional[5] |
        moves.directional[6] |
        moves.directional[7];

    return moves;
}

Board Board::move(int i) {
    uint64_t sel = self.raw(), opp = opponent.raw();
    
    int ro = i / 8 * 8;

    uint64_t mask_si = (1ULL << i);
    uint64_t opps = opp | mask_si;
    
    uint64_t mask_hl = mask_si;
    uint64_t mask_hr = mask_si;
    uint64_t mask_vu = mask_si;
    uint64_t mask_vd = mask_si;

    for (int j = i - 1; j >= ro; j--) {
        mask_hl |= (mask_hl >> 1);
        if ((mask_hl & opps) != mask_hl) break;
    }
    for (int j = i + 1; j < ro + 8; j++) {
        mask_hr |= (mask_hr << 1);
        if ((mask_hr & opps) != mask_hr) break;
    }

    for (int j = i - 8; j >= 0; j -= 8) {
        mask_vu |= (mask_vu >> 8);
        if ((mask_vu & opps) != mask_vu) break;
    }
    for (int j = i + 8; j < 64; j += 8) {
        mask_vd |= (mask_vd << 8);
        if ((mask_vd & opps) != mask_vd) break;
    }

    int x = i % 8;
    int y = i / 8;

    int mdul = x < y ? x : y;
    int mdur = (7 - x) < y ? (7 - x) : y;
    int mddl = x < (7 - y) ? x : (7 - y);
    int mddr = (7 - x) < (7 - y) ? (7 - x) : (7 - y);
    
    uint64_t mask_ul = mask_si;
    uint64_t mask_ur = mask_si;
    uint64_t mask_dl = mask_si;
    uint64_t mask_dr = mask_si;

    for (int j = 0; j < mdul; j++) {
        mask_ul |= (mask_ul >> 9);
        if ((mask_ul & opps) != mask_ul) break;
    }
    for (int j = 0; j < mdur; j++) {
        mask_ur |= (mask_ur >> 7);
        if ((mask_ur & opps) != mask_ur) break;
    }
    for (int j = 0; j < mddl; j++) {
        mask_dl |= (mask_dl << 7);
        if ((mask_dl & opps) != mask_dl) break;
    }
    for (int j = 0; j < mddr; j++) {
        mask_dr |= (mask_dr << 9);
        if ((mask_dr & opps) != mask_dr) break;
    }

    uint64_t mask_oi = sel | mask_si;

    if ((mask_hl & ((opp & MOVE_L_MASK) | mask_oi)) != mask_hl) mask_hl = 0;
    if ((mask_hr & ((opp & MOVE_R_MASK) | mask_oi)) != mask_hr) mask_hr = 0;
    if ((mask_vu & ((opp & MOVE_U_MASK) | mask_oi)) != mask_vu) mask_vu = 0;
    if ((mask_vd & ((opp & MOVE_D_MASK) | mask_oi)) != mask_vd) mask_vd = 0;
    if ((mask_ul & ((opp & MOVE_UL_MASK) | mask_oi)) != mask_ul) mask_ul = 0;
    if ((mask_ur & ((opp & MOVE_UR_MASK) | mask_oi)) != mask_ur) mask_ur = 0;
    if ((mask_dl & ((opp & MOVE_DL_MASK) | mask_oi)) != mask_dl) mask_dl = 0;
    if ((mask_dr & ((opp & MOVE_DR_MASK) | mask_oi)) != mask_dr) mask_dr = 0;

    uint64_t mask = mask_hl | mask_hr | mask_vu | mask_vd |
                    mask_ul | mask_ur | mask_dl | mask_dr;

    return Board(sel | mask, opp & ~mask);
}
