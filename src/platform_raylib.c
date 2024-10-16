#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "core/types.h"
#include "core/platform.h"

#define TYPE_CAST(type, value) *(type*)(&value)
#define U32_TO_COLOR(c) TYPE_CAST(Color, c)

static Font font;

#define TEXTURE_BUFFER_SIZE (SPRT_FLAG - SPRT_PUMPKIN + 1)
static bool texture_usage_map[TEXTURE_BUFFER_SIZE] = {false};
static Image texture_buffer_img[TEXTURE_BUFFER_SIZE] = {0};
static Texture2D texture_buffer[TEXTURE_BUFFER_SIZE] = {0};

i32 platform_rand()
{
	return rand();
}

void platform_init_window(u32 width, u32 height, const char* title)
{
	InitWindow(width, height, title);
	font = GetFontDefault();
}

bool platform_window_should_close()
{
	return WindowShouldClose();
}

void platform_begin_drawing()
{
	BeginDrawing();
}

void platform_end_drawing()
{
	EndDrawing();
}

void platform_clear_background(u32 color)
{
	ClearBackground(U32_TO_COLOR(color));
}

u32 platform_measure_text(const char* text, u32 font_size)
{
	return MeasureText(text, font_size);
}

vec2_t platform_measure_text_ex(const char* text, u32 font_size, u32 spacing)
{
	Vector2 v2 = MeasureTextEx(font, text, font_size, spacing);
	return (vec2_t) {
		.x = v2.x,
		.y = v2.y,
	};
}

void platform_draw_rectangle(const f32 x, const f32 y, const f32 w, const f32 h, const u32 color)
{
	DrawRectangle(x, y, w, h, U32_TO_COLOR(color));
}

void platform_draw_texture(const texture_t tex, const vec2_t pos, u32 tint)
{
	if (!texture_usage_map[tex.id]) {
		texture_buffer_img[tex.id] = (Image){
			.mipmaps = 1,
			.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
			.width = tex.width,
			.height = tex.height,
			.data = tex.data,
		};

		texture_buffer[tex.id] = LoadTextureFromImage(texture_buffer_img[tex.id]);
		texture_usage_map[tex.id] = true;
	}

	DrawTexture(texture_buffer[tex.id], pos.x, pos.y, U32_TO_COLOR(tint));
}

void platform_draw_text(const char* text, u32 x, u32 y, u32 font_size, u32 color)
{
	DrawText(text, x, y, font_size, U32_TO_COLOR(color));
}

void platform_draw_text_ex(const char* text, f32 x, f32 y, u32 font_size, u32 spacing, u32 color)
{
	DrawTextEx(font, text, (Vector2){x, y}, font_size, spacing, U32_TO_COLOR(color));
}

vec2_t platform_get_mouse_position()
{
	Vector2 mouse_pos = GetMousePosition();
	return (vec2_t) {
		.x = mouse_pos.x,
		.y = mouse_pos.y,
	};
}

bool platform_is_mouse_button_pressed(mouse_button_e mouse_button)
{
	i32 raylib_btn;
	switch(mouse_button) {
		case MOUSE_LEFT:
			raylib_btn = MOUSE_LEFT_BUTTON;
			break;
		case MOUSE_RIGHT:
			raylib_btn = MOUSE_RIGHT_BUTTON;
			break;
	}
	return IsMouseButtonPressed(raylib_btn);
}
