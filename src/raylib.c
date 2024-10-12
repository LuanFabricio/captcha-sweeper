#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

#include "core/platform.h"
#include "core/types.h"
#include "core/minesweeper.h"
#include "core/captcha.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define GRID_SIZE 64
#define GRID_PAD 2

// #define GRID_OFFSET_X 220
// #define GRID_OFFSET_Y 220

#define GRID_WIDTH 6
#define GRID_HEIGHT 6
#define GRID_LENGTH GRID_WIDTH * GRID_HEIGHT

#define GRID_SCREEN_WIDTH (GRID_SIZE + GRID_PAD) * GRID_WIDTH
#define GRID_SCREEN_HEIGHT (GRID_SIZE + GRID_PAD) * GRID_HEIGHT

const u32 GRID_OFFSET_X = (SCREEN_WIDTH - GRID_SCREEN_WIDTH) / 2;
const u32 GRID_OFFSET_Y = (SCREEN_HEIGHT - GRID_SCREEN_HEIGHT) / 2;

const u32 GRID_MIN_X = GRID_OFFSET_X;
const u32 GRID_MIN_Y = GRID_OFFSET_Y;

const u32 GRID_MAX_X = GRID_OFFSET_X + (GRID_SIZE + GRID_PAD) * GRID_WIDTH;
const u32 GRID_MAX_Y = GRID_OFFSET_Y + (GRID_SIZE + GRID_PAD) * GRID_HEIGHT;

const u32 UNMASK_NO_BOMB_COLOR = 0xffffffff;
const u32 UNMASK_BOMB_COLOR = 0xff0000ff;
const u32 MASK_COLOR = 0xff383838;

#define RESET_RECTANGLE_WIDTH 280
#define RESET_RECTANGLE_HEIGHT 140
#define RESET_RECTANGLE_BORDER_SIZE 10

const Rectangle RESET_RECTANGLE = {
	.x = (SCREEN_WIDTH - RESET_RECTANGLE_WIDTH) / 2,
	.y = (SCREEN_HEIGHT - RESET_RECTANGLE_HEIGHT) / 2,
	.width = RESET_RECTANGLE_WIDTH,
	.height = RESET_RECTANGLE_HEIGHT,
};

const Rectangle RESET_RECTANGLE_BORDER = {
	.x = (SCREEN_WIDTH - RESET_RECTANGLE_WIDTH - RESET_RECTANGLE_BORDER_SIZE) / 2,
	.y = (SCREEN_HEIGHT - RESET_RECTANGLE_HEIGHT - RESET_RECTANGLE_BORDER_SIZE) / 2,
	.width = RESET_RECTANGLE_WIDTH + RESET_RECTANGLE_BORDER_SIZE,
	.height = RESET_RECTANGLE_HEIGHT + RESET_RECTANGLE_BORDER_SIZE,
};

typedef struct {
	Font font;
	u32 font_size, spacing;
	char* game_over_text;
	Vector2 game_over_position, game_over_size;
	char* reset_text;
	Vector2 reset_position, reset_size;
	Rectangle reset_rect;
} reset_ui_content_t;

reset_ui_content_t init_reset_ui_content()
{

	const Font font = GetFontDefault();
	const u32 font_size = 32;
	const u32 spacing = font_size / 10; // font_size / default_font_size

	const char* game_over = "GAME OVER!";
	const Vector2 game_over_size = MeasureTextEx(font, game_over, font_size, spacing);
	const Vector2 game_over_position = {
		(SCREEN_WIDTH - game_over_size.x) / 2,
		SCREEN_HEIGHT / 2 - 45,
	};

	const char* reset = "RESET";
	const Vector2 reset_size = MeasureTextEx(font, reset, font_size, spacing);
	const Vector2 reset_position = {
		(SCREEN_WIDTH - reset_size.x) / 2,
		SCREEN_HEIGHT / 2 + 15,
	};

	Rectangle reset_rect = {
		.x = reset_position.x,
		.y = reset_position.y,
		.width = reset_size.x,
		.height = reset_size.y,
	};

	return (reset_ui_content_t){
		.font = font,
		.font_size = 32,
		.spacing = spacing,
		.game_over_text = "GAME OVER!",
		.game_over_position = game_over_position,
		.game_over_size = game_over_size,
		.reset_text = "RESET",
		.reset_position = reset_position,
		.reset_size = reset_size,
		.reset_rect = reset_rect,
	};
}

Vector2 world_to_grid(Vector2 position)
{
	Vector2 res = { -1, -1 };

	if (position.x < GRID_MIN_X || position.x > GRID_MAX_X) return res;
	if (position.y < GRID_MIN_Y || position.y > GRID_MAX_Y) return res;

	res.x = ((u32)position.x - GRID_MIN_X) / (GRID_SIZE + GRID_PAD);
	res.y = ((u32)position.y - GRID_MIN_Y) / (GRID_SIZE + GRID_PAD);
	return res;
}

void draw_minesweep(const minesweep_t minesweep, const Texture2D flagTexture)
{
	u32 x = GRID_OFFSET_X;
	u32 y = GRID_OFFSET_Y;

	for (u32 i = 0; i < GRID_LENGTH; i++) {
		if (i % minesweep.width == 0 && i != 0) {
			y += GRID_SIZE + GRID_PAD;
			x = GRID_OFFSET_X;
		}

		const u32 grid_index = i / GRID_TYPE_BITS;
		const u32 mask = minesweep.mask[grid_index] >> i & 1;
		u32 color = MASK_COLOR;

		if (mask) {
			const u32 grid_value = minesweep.grid[grid_index] >> i & 1;
			if (grid_value) color = UNMASK_BOMB_COLOR;
			else color = UNMASK_NO_BOMB_COLOR;
		}

		DrawRectangle(x, y, GRID_SIZE, GRID_SIZE, *(Color*)&color);

		const u32 mark = minesweep.mark[grid_index] >> i & 1;
		if (mark) DrawTexture(flagTexture, x + flagTexture.width / 2, y, WHITE);

		x += GRID_SIZE + GRID_PAD;
	}

}

void draw_reset_ui(reset_ui_content_t reset_content, bool is_reset_selected)
{
	const u32 font_size = 32;
	const u32 spacing = font_size / 10; // font_size / default_font_size

	const char* game_over = "GAME OVER!";
	const u32 game_over_width = MeasureText(game_over, font_size);
	const Vector2 game_over_position = {
		(SCREEN_WIDTH - game_over_width) / 2,
		SCREEN_HEIGHT / 2 - 45,
	};


	const char* reset = "RESET";
	const u32 reset_width = MeasureText(reset, font_size);
	const Vector2 reset_position = {
		(SCREEN_WIDTH - reset_width) / 2,
		SCREEN_HEIGHT / 2 + 15,
	};

	DrawRectangleRec(RESET_RECTANGLE_BORDER, GRAY);
	DrawRectangleRec(RESET_RECTANGLE, BLACK);
	DrawTextEx(reset_content.font,
			reset_content.game_over_text,
			reset_content.game_over_position,
			reset_content.font_size,
			reset_content.spacing, WHITE);

	const Color reset_color = is_reset_selected ? YELLOW : WHITE;
	DrawTextEx(reset_content.font,
			reset_content.reset_text,
			reset_content.reset_position,
			reset_content.font_size,
			reset_content.spacing, reset_color);

	// DrawRectangleLinesEx(reset_content.reset_rect, 1.f, GREEN);
}

i32 platform_rand()
{
	return rand();
}

int main(void)
{
	srand(time(NULL));

	minesweep_t minesweep = minesweep_new(GRID_WIDTH, GRID_HEIGHT);
	minesweep_create_random_grid(minesweep.grid, GRID_WIDTH, GRID_HEIGHT);
	captcha_t captcha = captcha_create(minesweep);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CaptchaSweeper");
	// minesweep.mask[0] = -1;

	reset_ui_content_t reset_contet = init_reset_ui_content();
	bool is_reset_selected = false;
	Texture2D pumpkin = LoadTexture("assets/sprites/pumpkin.png");
	Texture2D flag = LoadTexture("assets/sprites/flag.png");

	while(!WindowShouldClose()) {
		const bool is_game_over = minesweep_is_game_done(captcha.minesweep);
		const bool player_win = minesweep_player_won(captcha.minesweep);

		BeginDrawing();
		ClearBackground((Color){ .r=0, .g=0, .b=0, .a=0xff});


		switch (captcha.state.current_state) {
			case STATE_ON_CAPTCHA:
				{
					draw_minesweep(captcha.minesweep, flag);
					if (is_game_over) {
						draw_reset_ui(reset_contet, is_reset_selected);
					}
				}
				break;
			case STATE_CONFIRMED:
				{
					DrawTexture(pumpkin, 300, 300, WHITE);
					DrawText("Game over :)", 300, 300, 32, GREEN);
				}
				break;
		}

		EndDrawing();

		const Vector2 mouse_position = GetMousePosition();
		const bool collide_x = mouse_position.x > reset_contet.reset_rect.x
			&& mouse_position.x <= reset_contet.reset_rect.x + reset_contet.reset_rect.width;
		const bool collide_y = mouse_position.y > reset_contet.reset_rect.y
			&& mouse_position.y <= reset_contet.reset_rect.y + reset_contet.reset_rect.height;

		if (is_game_over) {
			is_reset_selected = collide_x && collide_y;
		}

		Vector2 position = world_to_grid(mouse_position);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (is_game_over && is_reset_selected) {
				minesweep_reset(&captcha.minesweep);
			} else if (!player_win) {
				if (position.x != -1.f || position.y != -1.f) {
					minesweep_flip_position(&captcha.minesweep,
							(u32)position.x, (u32)position.y);
					if (minesweep_is_game_done(captcha.minesweep)) {
						minesweep_flip_all(&captcha.minesweep);
					} else if (minesweep_player_won(captcha.minesweep)) {
						state_next(&captcha.state);
					}
				}
			}
		} else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			minesweep_set_mark(&captcha.minesweep, position.x, position.y);
		}
	}

	return 0;
}
