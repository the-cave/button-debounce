# Button Debounce

## What is it?

Button Debounce is a library to handle [contact bounce](https://en.wikipedia.org/wiki/Switch#Contact_bounce) in your microcontroller system circuit. It build with event driven architecture, non-blocking execution, only standard C, platform and hardware independent. And I hope you'd enjoy it.

## Example usage

This is an example usage with an STM8 microcontroller.  
The library itself is platform and hardware independent you can use it with Arduino, MCS-51, ARM Cortex-M, you name it.
The example uses the library to detect button press signal and toggle the LED on the hardware it runs on.
```c
#define STM8S103
#include "stm8s.h"

// the library is namespaced, name clashing hardly occurs
#define BUTTON_DEBOUNCE__CONFIRM 64
#include "button_debounce.c"

// configure callbacks
const ButtonDebounce_Config button_c3_debounce_config = {
    .fell = &led_toggle, // "fell" event subscription
    // apart from "fell"
    // there are "rose" and "state_changed" events
};
static ButtonDebounce_State button_c3_debounce_state;

int main() {
  uint8_t prescaler;

  // configure pins
  SetBit(GPIOB->DDR, 5);
  SetBit(GPIOB->ODR, 5);
  SetBit(GPIOC->CR1, 3);

  button_debounce__state_init(&button_c3_debounce_state);

#define _PRESCALER_DIVIDE_64 (64 - 1)

  for (prescaler = 0;; prescaler++) {
    // sampling the state of the button on pin C3
    if (!(prescaler & _PRESCALER_DIVIDE_64)) {
      button_debounce__sample(
        &button_c3_debounce_config,
        &button_c3_debounce_state,
        ValBit(GPIOC->IDR, 3)
      );
    }
  }
}

void led_toggle() {
  // toggle an LED on pin B5
  ChgBit(GPIOB->ODR, 5);
}
```
See the whole project including Makefile and build instructions at https://github.com/midnight-wonderer/button-debounce-example

## License

Button Debounce is released under the [MIT License](https://opensource.org/licenses/MIT).
