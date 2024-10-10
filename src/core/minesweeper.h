#ifndef __MINESWEEP_H__
#define __MINESWEEP_H__

#include "types.h"

#define GRID_TYPE_BITS 32
#define MAX_BOMBS 6

typedef struct {
	u32 width, height;
	u32 *grid, *mask, *mark;
} minesweep_t;

void minesweep_print(minesweep_t minesweep);
minesweep_t minesweep_new(u32 width, u32 height, u32 *grid);
u32* minesweep_create_random_grid(u32 width, u32 height);
void minesweep_reset(minesweep_t* minesweep);
void minesweep_flip_position(minesweep_t *minesweep, u32 x, u32 y);
void minesweep_flip_blank_neightbors(minesweep_t *minesweep, u32 x, u32 y, u32 current_deep, u32 max_deep);
void minesweep_flip_all(minesweep_t *minesweep);
void minesweep_set_mark(minesweep_t *minesweep, u32 x, u32 y);
bool minesweep_is_game_done(const minesweep_t minesweep);
bool minesweep_player_won(const minesweep_t minesweep);

#endif // __MINESWEEP__H_
