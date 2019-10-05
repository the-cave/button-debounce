// Copyright (c) 2018 Sarun Rattanasiri
// under the MIT License https://opensource.org/licenses/MIT

#include "button_debounce.h"

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
          if (config->fell) {
            config->fell();
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
          if (config->rose) {
            config->rose();
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
