#include "raylib.h"

#include "../src/types.h"
#include "../src/minesweeper.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define GRID_SIZE 32
#define GRID_PAD 2

int main(void)
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CaptchaSweeper");


	const u32 grid_width = 8;
	const u32 grid_height = 8;
	minesweep_t minesweep = minesweep_new(grid_width, grid_height, minesweep_create_random_grid(grid_width, grid_height));
	minesweep.mask[0] = -1;

	const u32 grid_length = grid_width * grid_height;
	u32 x, y;

	const u32 unmask_no_bomb_color = 0xffffffff;
	const u32 unmask_bomb_color = 0xff0000ff;
	const u32 mask_color = 0xff181818;
	while(!WindowShouldClose()) {
		x = 0;
		y = 0;

		BeginDrawing();
		ClearBackground((Color){ .r=0, .g=0, .b=0, .a=0xff});
		for (u32 i = 0; i < grid_length; i++) {
			if (i % minesweep.width == 0) {
				y += GRID_SIZE+ GRID_PAD;
				x = 0;
			}

			const u32 grid_index = i / GRID_TYPE_BITS;
			const u32 mask = minesweep.mask[grid_index] >> i & 1;
			u32 color = mask_color;

			if (mask) {
				const u32 grid_value = minesweep.grid[grid_index] >> i & 1;
				if (grid_value) color = unmask_bomb_color;
				else color = unmask_no_bomb_color;
			}

			DrawRectangle(x, y, GRID_SIZE, GRID_SIZE, *(Color*)&color);

			x += GRID_SIZE + GRID_PAD;
		}
		EndDrawing();
	}

	return 0;
}
