#include <stdio.h>
#include "raylib.h"

#include "../src/types.h"
#include "../src/minesweeper.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define GRID_SIZE 32
#define GRID_PAD 2

#define GRID_OFFSET_X 220
#define GRID_OFFSET_Y 220

#define GRID_WIDTH 8
#define GRID_HEIGHT 8

const u32 GRID_MIN_X = GRID_OFFSET_X;
const u32 GRID_MIN_Y = GRID_OFFSET_Y;

const u32 GRID_MAX_X = GRID_OFFSET_X + (GRID_SIZE + GRID_PAD) * GRID_WIDTH;
const u32 GRID_MAX_Y = GRID_OFFSET_Y + (GRID_SIZE + GRID_PAD) * GRID_HEIGHT;

Vector2 world_to_grid(Vector2 position)
{
	Vector2 res = { -1, -1 };

	if (position.x < GRID_MIN_X || position.x > GRID_MAX_X) return res;
	if (position.y < GRID_MIN_Y || position.y > GRID_MAX_Y) return res;

	res.x = ((u32)position.x - GRID_MIN_X) / (GRID_SIZE + GRID_PAD);
	res.y = ((u32)position.y - GRID_MIN_Y) / (GRID_SIZE + GRID_PAD);
	return res;
}

int main(void)
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CaptchaSweeper");

	minesweep_t minesweep = minesweep_new(GRID_WIDTH, GRID_HEIGHT, minesweep_create_random_grid(GRID_WIDTH, GRID_HEIGHT));
	minesweep.mask[0] = -1;

	const u32 grid_length = GRID_WIDTH * GRID_HEIGHT;
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
			if (i % minesweep.width == 0 && i != 0) {
				y += GRID_SIZE + GRID_PAD;
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

			DrawRectangle(x + GRID_OFFSET_X, y + GRID_OFFSET_Y, GRID_SIZE, GRID_SIZE, *(Color*)&color);

			x += GRID_SIZE + GRID_PAD;
		}
		EndDrawing();

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 position = world_to_grid(GetMousePosition());
			if (position.x != -1.f || position.y != -1.f)
				minesweep_flip_position(&minesweep, (u32)position.x, (u32)position.y);
		}
	}

	return 0;
}
