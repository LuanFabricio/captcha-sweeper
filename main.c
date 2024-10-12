#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "src/core/platform.h"
#include "src/core/types.h"
#include "src/core/minesweeper.h"

i32 platform_rand()
{
	return rand();
}

int main(void)
{
	srand(time(NULL));
	const u32 WIDTH = 8;
	const u32 HEIGHT = 8;
	minesweep_t minesweep = minesweep_new(WIDTH, HEIGHT);
	minesweep_create_random_grid(minesweep.grid, WIDTH, HEIGHT);

	u32 x, y;
	while(!minesweep_is_game_done(minesweep)) {
		minesweep_print(minesweep);
		scanf("%u %u", &x, &y);
		minesweep_flip_position(&minesweep, x, y);
	}
	minesweep_print(minesweep);

	return 0;
}
