#ifndef __CAPTCHA_H__
#define __CAPTCHA_H__

#include "types.h"

typedef struct {
	u32 width, height;
	u32 *grid, *mask;
} minesweep_t;

void minesweep_print(minesweep_t minesweep);
minesweep_t minesweep_new(u32 width, u32 height, u32 *grid);
u32* minesweep_create_random_grid(u32 width, u32 height);
void minesweep_flip_position(minesweep_t *minesweep, u32 x, u32 y);
void minesweep_flip_blank_neightbors(minesweep_t *minesweep, u32 x, u32 y, u32 current_deep, u32 max_deep);
boolean minesweep_is_game_done(const minesweep_t minesweep);

#endif // __CAPTCHA__H_
