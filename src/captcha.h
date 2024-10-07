#ifndef __CAPTCHA_H__
#define __CAPTCHA_H__

#include "types.h"
#include "minesweeper.h"
#include "state.h"

typedef struct {
	minesweep_t minesweep;
	state_t state;
} captcha_t;

captcha_t captcha_create(minesweep_t minesweep);

#endif // __CAPTCHA_H__
