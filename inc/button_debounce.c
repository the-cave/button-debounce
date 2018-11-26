// Copyright (c) 2018 Sarun Rattanasiri
// under the MIT License https://opensource.org/licenses/MIT

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef BUTTON_DEBOUNCE__CONFIRM
#define BUTTON_DEBOUNCE__CONFIRM 200
#endif

#ifndef __BUTTON_DEBOUNCE__
#define __BUTTON_DEBOUNCE__

typedef enum _button_debounce__button_state {
  BUTTON_DEBOUNCE__LOW,
  BUTTON_DEBOUNCE__HIGH,
} ButtonDebounce_ButtonStatus;

typedef struct _button_debounce__config {
  void (*state_changed)(ButtonDebounce_ButtonStatus state);
  void (*rised)();
  void (*falled)();
} ButtonDebounce_Config;

typedef enum _button_debounce__internal_status {
  _BUTTON_DEBOUNCE__UNKNOWN,
  _BUTTON_DEBOUNCE__UNKNOWN_TO_HIGH,
  _BUTTON_DEBOUNCE__UNKNOWN_TO_LOW,
  _BUTTON_DEBOUNCE__HIGH,
  _BUTTON_DEBOUNCE__LOW,
  _BUTTON_DEBOUNCE__HIGH_TO_LOW,
  _BUTTON_DEBOUNCE__LOW_TO_HIGH,
} _ButtonDebounce_InternalStatus;

typedef struct _button_debounce__state {
  _ButtonDebounce_InternalStatus status;
  uint8_t confirmation_count;
} ButtonDebounce_State;

#define button_debounce__state_init(state)       \
  do {                                           \
    (state)->status = _BUTTON_DEBOUNCE__UNKNOWN; \
    (state)->confirmation_count = 0;             \
  } while (0)

void button_debounce__sample(ButtonDebounce_Config* config,
                             ButtonDebounce_State* state,
                             uint8_t button_state) {
  switch (state->status) {
    case _BUTTON_DEBOUNCE__HIGH:
      if (!button_state) {
        state->status = _BUTTON_DEBOUNCE__HIGH_TO_LOW;
        state->confirmation_count = 0;
      }
      break;
    case _BUTTON_DEBOUNCE__LOW:
      if (button_state) {
        state->status = _BUTTON_DEBOUNCE__LOW_TO_HIGH;
        state->confirmation_count = 0;
      }
      break;
    case _BUTTON_DEBOUNCE__HIGH_TO_LOW:
      if (button_state) {
        state->status = _BUTTON_DEBOUNCE__HIGH;
      } else {
        if (state->confirmation_count++ > BUTTON_DEBOUNCE__CONFIRM) {
          state->status = _BUTTON_DEBOUNCE__LOW;
          if (config->falled) {
            config->falled();
          }
          if (config->state_changed) {
            config->state_changed(BUTTON_DEBOUNCE__LOW);
          }
        }
      }
      break;
    case _BUTTON_DEBOUNCE__LOW_TO_HIGH:
      if (button_state) {
        if (state->confirmation_count++ > BUTTON_DEBOUNCE__CONFIRM) {
          state->status = _BUTTON_DEBOUNCE__HIGH;
          if (config->rised) {
            config->rised();
          }
          if (config->state_changed) {
            config->state_changed(BUTTON_DEBOUNCE__HIGH);
          }
        }
      } else {
        state->status = _BUTTON_DEBOUNCE__LOW;
      }
      break;
    case _BUTTON_DEBOUNCE__UNKNOWN:
      if (button_state) {
        state->status = _BUTTON_DEBOUNCE__UNKNOWN_TO_HIGH;
      } else {
        state->status = _BUTTON_DEBOUNCE__UNKNOWN_TO_LOW;
      }
      state->confirmation_count = 0;
      break;
    case _BUTTON_DEBOUNCE__UNKNOWN_TO_HIGH:
      if (button_state) {
        if (state->confirmation_count++ > BUTTON_DEBOUNCE__CONFIRM) {
          state->status = _BUTTON_DEBOUNCE__HIGH;
          if (config->state_changed) {
            config->state_changed(BUTTON_DEBOUNCE__HIGH);
          }
        }
      } else {
        state->status = _BUTTON_DEBOUNCE__UNKNOWN;
      }
      break;
    case _BUTTON_DEBOUNCE__UNKNOWN_TO_LOW:
      if (button_state) {
        state->status = _BUTTON_DEBOUNCE__UNKNOWN;
      } else {
        if (state->confirmation_count++ > BUTTON_DEBOUNCE__CONFIRM) {
          state->status = _BUTTON_DEBOUNCE__LOW;
          if (config->state_changed) {
            config->state_changed(BUTTON_DEBOUNCE__LOW);
          }
        }
      }
  }
}

#endif
