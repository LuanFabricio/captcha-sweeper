#ifndef __STATE_H__
#define __STATE_H__

typedef enum {
	STATE_ON_CAPTCHA = 0, // MIN
	STATE_CONFIRMED 	// MAX
} state_enum;

typedef struct state_t {
	state_enum transition_map[STATE_CONFIRMED-STATE_ON_CAPTCHA+1];
	state_enum current_state;
} state_t;

state_t state_create();
void state_next(state_t *state);

#endif // __STATE_H__
