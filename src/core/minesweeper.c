#ifndef WEB
#include <stdio.h>
#endif

#include "platform.h"
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

bool is_valid_point(u32 x, u32 y, u32 width, u32 height)
{
	return x < width && y < height;
}

void flip_value(minesweep_t* minesweep, u32 index, u32 bit_position)
{
	minesweep->mask[index] |= 1 << bit_position;
	minesweep->mark[index] &= ((1 << bit_position) ^ -1);
}

#ifndef WEB
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
		//printf("(%02u) ", grid_index);
	}
	printf("\n");
}
#endif

minesweep_t minesweep_new(const u32 width, const u32 height)
{
	const u32 num_instances = number_of_instances(width, height);
	minesweep_t minesweep = {0};
	minesweep.width = width;
	minesweep.height = height;

	return minesweep;
}

void minesweep_reset(minesweep_t* minesweep)
{
	for (u32 i = 0; i < number_of_instances(minesweep->width, minesweep->width); i++) {
		minesweep->mask[i] = 0;
		minesweep->mark[i] = 0;
	}
	minesweep_create_random_grid(minesweep->grid, minesweep->width, minesweep->height);
}

void minesweep_create_random_grid(u32 *grid, const u32 width, const u32 height)
{
	const u32 num_instances = number_of_instances(width, height);

	const u32 grid_size = width * height;
	for (u32 i = 0; i < num_instances; i++) grid[i] = 0;

	for (u32 i = 0; i < MAX_BOMBS; i++) {
		const u32 x = platform_rand() % width;
		const u32 y = platform_rand() % height;

		const u32 index = xy_to_index(x, y, width);
		const u32 bit = xy_to_bit_position(x, y, width);
		grid[index] |= 1 << bit;
	}
}

void minesweep_flip_position(minesweep_t *minesweep, u32 x, u32 y)
{
	if (!is_valid_point(x, y, minesweep->width, minesweep->height)) return;

	const u32 index = xy_to_index(x, y, minesweep->width);
	const u32 bit_position = xy_to_bit_position(x, y, minesweep->width);
	flip_value(minesweep, index, bit_position);

	if (minesweep->grid[index] & 1 << bit_position) return;

	const u32 current_deep = 0;
	const u32 max_deep = 4;
	minesweep_flip_blank_neightbors(minesweep, x+1, y, current_deep+1, max_deep);
	minesweep_flip_blank_neightbors(minesweep, x, y+1, current_deep+1, max_deep);
	if (x > 0) minesweep_flip_blank_neightbors(minesweep, x-1, y, current_deep+1, max_deep);
	if (y > 0) minesweep_flip_blank_neightbors(minesweep, x, y-1, current_deep+1, max_deep);
}

void minesweep_flip_blank_neightbors(minesweep_t *minesweep, u32 x, u32 y, u32 current_deep, u32 max_deep)
{
	if (current_deep >= max_deep) return;
	if (!is_valid_point(x, y, minesweep->width, minesweep->height)) return;

	u32 index = xy_to_index(x, y, minesweep->width);
	u32 bit_position = xy_to_bit_position(x, y, minesweep->width);

	if (minesweep->grid[index] & 1 << bit_position) return;
	if (minesweep->mask[index] & 1 << bit_position) return;
	flip_value(minesweep, index, bit_position);

	minesweep_flip_blank_neightbors(minesweep, x+1, y, current_deep+1, max_deep);
	minesweep_flip_blank_neightbors(minesweep, x, y+1, current_deep+1, max_deep);
	if (x > 0) minesweep_flip_blank_neightbors(minesweep, x-1, y, current_deep+1, max_deep);
	if (y > 0) minesweep_flip_blank_neightbors(minesweep, x, y-1, current_deep+1, max_deep);
}

void minesweep_flip_all(minesweep_t *minesweep)
{
	const u32 total_instances = number_of_instances(minesweep->width, minesweep->height);

	for (u32 i = 0; i < total_instances; i++) {
		minesweep->mask[i] = 0xffffffff;
		minesweep->mark[i] = 0;
	}
}

void minesweep_set_mark(minesweep_t *minesweep, u32 x, u32 y)
{
	if (!is_valid_point(x, y, minesweep->width, minesweep->height)) return;
	const u32 index = xy_to_index(x, y, minesweep->width);
	const u32 bit_position = xy_to_bit_position(x, y, minesweep->width);

	minesweep->mark[index] |= 1 << bit_position;
}

bool minesweep_is_game_done(const minesweep_t minesweep)
{
	const u32 total_size = minesweep.width * minesweep.height;
	for (u32 i = 0; i < total_size; i++) {
		const u32 grid_index = i / GRID_TYPE_BITS;
		const u32 mask = minesweep.mask[grid_index] >> i & 1;
		const u32 grid = minesweep.grid[grid_index] >> i & 1;
		if (mask && grid) return true;
	}
	return false;
}

bool minesweep_player_won(const minesweep_t minesweep)
{
	const u32 total_bits = minesweep.width * minesweep.height;
	u32 total_bombs = 0;
	u32 total_empty_unmasked = 0;
	for (u32 i = 0; i < total_bits; i++) {
		const u32 index = i / GRID_TYPE_BITS;
		const u32 grid = minesweep.grid[index] >> i & 1;
		if (grid) {
			total_bombs++;
			continue;
		}

		const u32 mask = minesweep.mask[index] >> i & 1;
		if (mask && grid == 0) total_empty_unmasked++;
	}

	return total_empty_unmasked + total_bombs == total_bits;
}
