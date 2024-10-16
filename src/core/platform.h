#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "types.h"

#define EXPAND_RECT(rect)rect.x, rect.y, rect.w, rect.h

i32 platform_rand();

void platform_init_window(u32 width, u32 height, const char* title);

bool platform_window_should_close();

void platform_begin_drawing();

void platform_end_drawing();

void platform_clear_background(u32 color);

u32 platform_measure_text(const char* text, u32 font_size);

vec2_t platform_measure_text_ex(const char* text, u32 font_size, u32 spacing);

void platform_draw_rectangle(const f32 x, const f32 y, const f32 w, const f32 h, const u32 color);

// TODO: Move signature to texture_bytes, width, height, x and y
void platform_draw_texture(const texture_t tex, const vec2_t pos, u32 tint);

void platform_draw_text(const char* text, u32 x, u32 y, u32 font, u32 color);

void platform_draw_text_ex(const char* text, f32 x, f32 y, u32 font_size, u32 spacing, u32 color);

vec2_t platform_get_mouse_position();

bool platform_is_mouse_button_pressed(mouse_button_e);

#endif // __PLATFORM_H__
