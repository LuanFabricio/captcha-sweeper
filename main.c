#include <stdio.h>

#include "src/core/types.h"
#include "src/core/minesweeper.h"

int main(void)
{
	const u32 WIDTH = 8;
	const u32 HEIGHT = 8;
	u32 *grid = minesweep_create_random_grid(WIDTH, HEIGHT);
	minesweep_t minesweep = minesweep_new(WIDTH, HEIGHT, grid);

	u32 x, y;
	while(!minesweep_is_game_done(minesweep)) {
		minesweep_print(minesweep);
		scanf("%u %u", &x, &y);
		minesweep_flip_position(&minesweep, x, y);
	}
	minesweep_print(minesweep);

	return 0;
}
