#include "state.h"

state_t state_create()
{
	return (state_t) {
		.transition_map = { STATE_CONFIRMED, STATE_ON_CAPTCHA },
		.current_state = STATE_ON_CAPTCHA,
	};
}

void state_next(state_t *state)
{
	const state_enum old_state = state->current_state;
	state->current_state = state->transition_map[old_state];
}
