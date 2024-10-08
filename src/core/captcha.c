#include "state.h"
#include "captcha.h"

captcha_t captcha_create(minesweep_t minesweep)
{
	return (captcha_t) {
		.minesweep = minesweep,
		.state = state_create(),
	};
}
