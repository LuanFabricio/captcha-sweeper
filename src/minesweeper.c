#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"
#include "minesweeper.h"

#define GRID_TYPE_BITS 32

u32 number_of_instances(const u32 width, const u32 height)
{
	const u32 grid_size = width * height;
	u32 num_instances = grid_size / GRID_TYPE_BITS;
	if (grid_size % GRID_TYPE_BITS) num_instances++;
	return num_instances;
}

u32 xy_to_index(const u32 x, const u32 y, const u32 width)
{
	return (y * width + x) / GRID_TYPE_BITS;
}

u32 xy_to_bit_position(const u32 x, const u32 y, const u32 width)
{
	return (y * width + x) % GRID_TYPE_BITS;
}

void minesweep_print(const minesweep_t minesweep)
{
	printf("Width: %u\n", minesweep.width);
	printf("Height: %u\n", minesweep.height);

	const u32 total_size = minesweep.width * minesweep.height;
	for(u32 i = 0; i < total_size ; i++) {
		if (i % minesweep.width == 0) {
			printf("\n");
		}
		const u32 grid_index = i / GRID_TYPE_BITS;
		const u32 mask = minesweep.mask[grid_index] >> i & 1;
		if (mask) {
			const u32 grid = minesweep.grid[grid_index] >> i & 1;
			printf("%u", grid);
		} else {
			printf("?");
		}
		printf("(%02u) ", grid_index);
	}
	printf("\n");
}

minesweep_t minesweep_new(const u32 width, const u32 height, u32 *grid)
{
	const u32 num_instances = number_of_instances(width, height);
	u32 *mask = (u32*)malloc(sizeof(u32) * num_instances);
	for (u32 i = 0; i < num_instances; i++) {
		mask[i] = 0;
	}

	return (minesweep_t) {
		.width = width,
			.height = height,
			.grid = grid,
			.mask = mask,
	};
}

u32* minesweep_create_random_grid(const u32 width, const u32 height)
{
	srand(time(NULL));
	const u32 num_instances = number_of_instances(width, height);
	u32 *grid = (u32*)malloc(sizeof(u32) * num_instances);

	const u32 grid_size = width * height;
	for (u32 i = 0; i < grid_size; i++) {
		if(rand() & 1) {
			const u32 grid_index = i / GRID_TYPE_BITS;
			grid[grid_index] |= 1 << i;
		}
	}

	return grid;
}

void minesweep_flip_position(minesweep_t *minesweep, u32 x, u32 y)
{
	const u32 index = xy_to_index(x, y, minesweep->width);
	const u32 bit_position = xy_to_bit_position(x, y, minesweep->width);
	minesweep->mask[index] |= 1 << bit_position;
}
