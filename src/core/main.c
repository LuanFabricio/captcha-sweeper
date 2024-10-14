#include "colors.h"
#include "platform.h"
#include "types.h"
#include "minesweeper.h"
#include "captcha.h"

#include "../../assets/sprites/flag.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define GRID_SIZE 64
#define GRID_PAD 2

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

const rectangle_t RESET_RECTANGLE = {
	.x = (SCREEN_WIDTH - RESET_RECTANGLE_WIDTH) / 2,
	.y = (SCREEN_HEIGHT - RESET_RECTANGLE_HEIGHT) / 2,
	.w = RESET_RECTANGLE_WIDTH,
	.h = RESET_RECTANGLE_HEIGHT,
};

const rectangle_t RESET_RECTANGLE_BORDER = {
	.x = (SCREEN_WIDTH - RESET_RECTANGLE_WIDTH - RESET_RECTANGLE_BORDER_SIZE) / 2,
	.y = (SCREEN_HEIGHT - RESET_RECTANGLE_HEIGHT - RESET_RECTANGLE_BORDER_SIZE) / 2,
	.w = RESET_RECTANGLE_WIDTH + RESET_RECTANGLE_BORDER_SIZE,
	.h = RESET_RECTANGLE_HEIGHT + RESET_RECTANGLE_BORDER_SIZE,
};

typedef struct {
	// Font font;
	u32 font_size, spacing;
	char* game_over_text;
	vec2_t game_over_position, game_over_size;
	char* reset_text;
	vec2_t reset_position, reset_size;
	rectangle_t reset_rect;
} reset_ui_content_t;

reset_ui_content_t init_reset_ui_content()
{
	reset_ui_content_t r = {0};
	r.font_size = 32;
	r.spacing = r.font_size / 10; // font_size / default_font_size
				      //
	r.game_over_text = "GAME OVER!";
	r.game_over_size = platform_measure_text_ex(r.game_over_text, r.font_size, r.spacing);
	r.game_over_position = (vec2_t){
		(SCREEN_WIDTH - r.game_over_size.x) / 2,
		SCREEN_HEIGHT / 2 - 45,
	};

	r.reset_text = "RESET";
	r.reset_size = platform_measure_text_ex(r.reset_text, r.font_size, r.spacing);
	r.reset_position = (vec2_t){
		(SCREEN_WIDTH - r.reset_size.x) / 2,
		SCREEN_HEIGHT / 2 + 15,
	};

	r.reset_rect = (rectangle_t){
		.x = r.reset_position.x,
		.y = r.reset_position.y,
		.w = r.reset_size.x,
		.h = r.reset_size.y,
	};

	return r;
}

vec2_t world_to_grid(vec2_t position)
{
	vec2_t res = { -1, -1 };

	if (position.x < GRID_MIN_X || position.x > GRID_MAX_X) return res;
	if (position.y < GRID_MIN_Y || position.y > GRID_MAX_Y) return res;

	res.x = ((u32)position.x - GRID_MIN_X) / (GRID_SIZE + GRID_PAD);
	res.y = ((u32)position.y - GRID_MIN_Y) / (GRID_SIZE + GRID_PAD);
	return res;
}

void draw_minesweep(const minesweep_t minesweep, const texture_t flagTexture)
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

		platform_draw_rectangle((rectangle_t){x, y, GRID_SIZE, GRID_SIZE}, color);

		const u32 mark = minesweep.mark[grid_index] >> i & 1;
		if (mark) platform_draw_texture(flagTexture, (vec2_t){x + flagTexture.width / 2, y}, WHITE);

		x += GRID_SIZE + GRID_PAD;
	}

}

void draw_reset_ui(reset_ui_content_t *reset_content, bool is_reset_selected)
{
	const u32 font_size = 32;
	const u32 spacing = font_size / 10; // font_size / default_font_size

	const char* game_over = "GAME OVER!";
	const u32 game_over_width = platform_measure_text(game_over, font_size);
	reset_content->game_over_position = (vec2_t){
		(SCREEN_WIDTH - game_over_width) / 2,
		SCREEN_HEIGHT / 2 - 45,
	};


	const char* reset = "RESET";
	const u32 reset_width = platform_measure_text(reset, font_size);
	reset_content->reset_position = (vec2_t){
		(SCREEN_WIDTH - reset_width) / 2.0,
		SCREEN_HEIGHT / 2.0 + 15,
	};

	platform_draw_rectangle(RESET_RECTANGLE_BORDER, GRAY);
	platform_draw_rectangle(RESET_RECTANGLE, BLACK);
	platform_draw_text_ex(reset_content->game_over_text,
			reset_content->game_over_position,
			reset_content->font_size,
			reset_content->spacing, WHITE);

	const u32 reset_color = is_reset_selected ? YELLOW : WHITE;
	platform_draw_text_ex(reset_content->reset_text,
			reset_content->reset_position,
			reset_content->font_size,
			reset_content->spacing, reset_color);

	// DrawrectangleLinesEx(reset_content.reset_rect, 1.f, GREEN);
}

int main(void)
{
	platform_init_window(SCREEN_WIDTH, SCREEN_HEIGHT, "CaptchaSweeper");

	minesweep_t minesweep = minesweep_new(GRID_WIDTH, GRID_HEIGHT);
	minesweep_create_random_grid(minesweep.grid, GRID_WIDTH, GRID_HEIGHT);
	captcha_t captcha = captcha_create(minesweep);

	// minesweep.mask[0] = -1;

	reset_ui_content_t reset_contet = init_reset_ui_content();
	bool is_reset_selected = false;

	texture_t pumpkin = {
		.id = SPRT_PUMPKIN,
		.width = 32,
		.height = 64,
		.data = assets_sprites_flag_raw,
	};

	texture_t flag = {
		.id = SPRT_FLAG,
		.width = 32,
		.height = 64,
		.data = assets_sprites_flag_raw,
	};
	// Texture2D pumpkin = LoadTexture("assets/sprites/pumpkin.png");
	// Texture2D flag = LoadTexture("assets/sprites/flag.png");

	while(!platform_window_should_close()) {
		const bool is_game_over = minesweep_is_game_done(captcha.minesweep);
		const bool player_win = minesweep_player_won(captcha.minesweep);

		platform_begin_drawing();
		platform_clear_background(0xff000000);

		switch (captcha.state.current_state) {
			case STATE_ON_CAPTCHA:
				{
					draw_minesweep(captcha.minesweep, flag);
					if (is_game_over) {
						draw_reset_ui(&reset_contet, is_reset_selected);
					}
				}
				break;
			case STATE_CONFIRMED:
				{
					platform_draw_texture(pumpkin, (vec2_t){300, 300}, WHITE);
					platform_draw_text("Game over :)", 300, 300, 32, GREEN);
				}
				break;
		}

		platform_end_drawing();

		const vec2_t mouse_position = platform_get_mouse_position();
		const bool collide_x = mouse_position.x > reset_contet.reset_rect.x
			&& mouse_position.x <= reset_contet.reset_rect.x + reset_contet.reset_rect.w;
		const bool collide_y = mouse_position.y > reset_contet.reset_rect.y
			&& mouse_position.y <= reset_contet.reset_rect.y + reset_contet.reset_rect.h;

		if (is_game_over) {
			is_reset_selected = collide_x && collide_y;
		}

		vec2_t position = world_to_grid(mouse_position);
		if (platform_is_mouse_button_pressed(MOUSE_LEFT)) {
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
		} else if (platform_is_mouse_button_pressed(MOUSE_RIGHT)) {
			minesweep_set_mark(&captcha.minesweep, position.x, position.y);
		}
	}

	return 0;
}
