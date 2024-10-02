#include <stdio.h>

#include "src/types.h"
#include "src/minesweeper.h"

int main(void)
{
	//u32 grid[] =  { 0x00 };//, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	//u32 mask[] =  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	const u32 WIDTH = 8;
	const u32 HEIGHT = 8;
	u32 *grid = minesweep_create_random_grid(WIDTH, HEIGHT);
	minesweep_t minesweep = minesweep_new(WIDTH, HEIGHT, grid);

	minesweep_flip_blank_neightbors(&minesweep, 1, 1, 0, 5);
	minesweep_print(minesweep);

	minesweep_flip_position(&minesweep, 1, 1);
	minesweep_print(minesweep);

	minesweep.mask[0] = -1;
	minesweep.mask[1] = -1;
	minesweep_print(minesweep);

	printf("Hello, world!\n");
	return 0;
}
