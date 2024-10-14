#ifndef __TYPES__H_
#define __TYPES__H_

typedef unsigned char u8;
typedef int i32;
typedef unsigned int u32;
typedef int i32;
typedef float f32;

typedef struct {
	f32 x, y;
} vec2_t;

typedef struct {
	f32 x, y;
	f32 w, h;
} rectangle_t;

typedef struct {
	u8 id;
	u32 width, height;
	u8 *data;
} texture_t;

typedef enum { MOUSE_LEFT, MOUSE_RIGHT } mouse_button_e;

typedef enum { SPRT_PUMPKIN, SPRT_FLAG } sprinte_e;

#ifndef __bool_true_false_are_defined
#define __bool_true_false_are_defined
typedef enum { false, true } bool;
#endif // __STDBOOL_H

#endif // __TYPES__H_
