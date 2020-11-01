# Arduino_straight_keyboard
Keyboard for a straight morse key using keyboard.h library

Note that an arduino capable of using the keyboard.h library (e.g. micro or leonardo) is required.

Note that for audible beeps, the output on pin 16 should be connected to a separate oscillator (e.g. 555 circuit). Alternatively, you can connect straight to a "buzzer" but this is not recommended.

Set up is extremely straightforward:

 - Straight key to -ve and pin 3 on the Arduino.
 - Remember to debounce the input with a resistor to +ve
 - Pin 16 should be connected to an led, pr buzze / oscillator circuit for audible beeps
