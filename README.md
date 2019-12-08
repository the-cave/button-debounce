# Button Debounce

## What is it?

Button Debounce is a library to handle [contact bounce](https://en.wikipedia.org/wiki/Switch#Contact_bounce) in your microcontroller system circuit. It build with event driven architecture, non-blocking execution, only standard C, platform and hardware independent. And I hope you'd enjoy it.  
I also write up about this library on [Slime Systems](https://slime.systems/content/the-best-contact-debouncer/), be sure to check it out.

## How to use this?

Just include the header to your project.
~~~c
#include "button_debounce.h"
~~~
Declaring what to do when the button press has been detected.
~~~c
// For example toggle an LED
void led_toggle() {
  // toggle an LED on pin B5
  ChgBit(GPIOB->ODR, 5);
}
~~~
Setup an object instance.
~~~c
const ButtonDebounce_Config button_c3_debounce_config = {
    .fell = &led_toggle, // point to the function we just declared

    // depend on the circuit and intentions
    // you might want to use .rose instead of .fell
    // which detects different edge transitions
};
static ButtonDebounce_State button_c3_debounce_state;

// somewhere in main()
button_debounce__init(&button_c3_debounce_state);
~~~
And keep sample the signal from the main loop.
~~~c
  for (;;) {
    button_debounce__sample(
      &button_c3_debounce_config,
      &button_c3_debounce_state,
      ValBit(GPIOC->IDR, 3)
    );
  }
~~~

See the whole project including Makefile and build instructions at https://github.com/midnight-wonderer/button-debounce-example

## License

Button Debounce is released under the [BSD 3-Clause License](LICENSE.md). :tada:
