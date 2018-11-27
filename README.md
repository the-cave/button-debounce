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

// it is a unconventional to include c directly without linking
// but this method suite the embedded system more IMHO
// this library design such that it is unlikely for name clashing
#include "button_debounce.c"

// configure callbacks
const ButtonDebounce_Config button_c3_debounce_config = {
    .falled = &led_toggle, // here we subscribe to "falled" event
    // apart from "falled" there are "rised" and "state_changed"
};
static ButtonDebounce_State button_c3_debounce_state;

int main() {
  // configure pins
  GPIOB->DDR = 0x20;
  GPIOC->CR1 = 0x08;
  GPIOB->ODR |= 0x20;

  button_debounce__state_init(&button_c3_debounce_state);

  for (;;) {
    // keep sample (poll) the state of the button and feed it to
    // the library
    button_debounce__sample(&button_c3_debounce_config,
                            &button_c3_debounce_state, GPIOC->IDR & 0x08);
  }
}

// subscribe to the event
void led_toggle() {
  // we just toggle an LED on pin B5 here
  GPIOB->ODR ^= 0x20;
}
```
See the whole project including Makefile and build instructions at https://github.com/midnight-wonderer/button-debounce-example

## License

Button Debounce is released under the [MIT License](https://opensource.org/licenses/MIT).
