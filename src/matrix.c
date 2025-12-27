#include "matrix.h" // imports our header file for matrix
#include "config.h" // imports our config constants
#include "pico/stdlib.h" // this imports pico sdk
#include <string.h> // imports C string library

static const uint8_t col_pins[] = {COL1_PIN, COL2_PIN, COL3_PIN};
// creates a static and constant list of the gpio pins of the cols
static const uint8_t row_pins[] = {ROW1_PIN, ROW2_PIN, ROW3_PIN};
// same thing as above but for rows
// note: these are constant meaning they are read only.
// we set both as uint8 types to save memory as gpios are [0, 29] inclusive, thus fitting within 8 bits

static uint16_t current_state = 0;
// im lazy so we're going to make a bit be each key -> bit 0 = key 1, bit 1 = key 2, ... , bit 8 = key 9
// we need 16 here bc 9 > 8 bits
static uint16_t previous_state = 0;
// this just sets the base previous state to zero
static uint32_t debounce_time[NUM_ROWS * NUM_COLS] = {0};
// this holds an array of 9 tampstamps which shows when each key last changed
// we are using the 32 version of int because miliseconds needs some more data
// we'll use this to enforce a debounce

void matrix_init(void) {
    for(int i = 0; i < NUM_COLS; i++) {
        gpio_init(col_pins[i]) // initializes the gpio pins of cols
        gpio_set_dir(col_pins[i], GPIO_OUT); // this sets these pins as output pins, meaning the MCU controls the voltage
        gpio_put(col_pins[i], 0); // this sets drive to low initially
    }

    for (int i = 0; i < NUM_ROWS; i++) {
        gpio_init(row_pins[i]) // initializes gpio pins of rows
        gpio_set_dir(row_pins[i], GPIO_IN); // this sets as pins as input pins, meaning the MCU detects the voltage relitve to ground
        gpio_pull_down(row_pins[i]); // this enables the pull down resistor
        // pull down resistors are a weak connection to ground
        // this keeps voltage at 0 when nothing is connected
        // when voltage is applied, the current takes the path of least resistance and goes into the pin
        // some energy is lost but eh
    }
}

uint16_t matrix_scan(void) {
    previous_state = current_state; // this makes the previous state equal to the current state (bc we are going to change current state)
    uint16_t new_state = 0; // defines an internal variable which we will work with, and will eventually set current state equal to
    uint32_t now = to_ms_since_boot(get_absolute_time()); // this tracks the debounce in miliseconds! these functions are in teh SDK. yay rpi :p

    for(int i = 0; i < NUM_COLS; i++) {
        gpio_put(col_pins[i], 1); // sets col pin to high
        sleep_us(10); // small delay for teh voltage to stabilize to high

        for(int j = 0; j < NUM_ROWS; j++) {
            uint8_t key_index = j * NUM_COLS + col; // simple algorithm to get the index of each 
            bool pressed = gpio_get(row_pins[j]); // this finds out if a specific key is pressed. if gpio_get finds a high voltage, we get true!

            if(now - debounce_time[key_index] > DEBOUNCE_MS) { // checks time between presses to prevent double presses
                bool was_pressed  = (current_state >> key_index) & 1; // this compares hardware reading vs stored state -> was_pressed is what the software thinks, pressed is the GPIO reading
                // okay this needs explaining because this is mildly complicated (bitshift!!!!)
                // so lets do an example where the key was pressed
                // let current state be 0b000010000 -> because 1 is in the 4th position, this menas that the 4th key was pressed!
                // now we shift it by the key index. lets say the current key index is 4, so after shifting 3 yielding 0b000000001
                // &1 masks thsi value now. 1 is 0b000000001, so, 0b000000001 & 0b000000001 -> 0b000000001 which is 1 which is true
                // this sets was_pressed to true!
                // now, let current state be 0b000010000 which means the fourth key was pressed
                // now lets say the key index is 3, so the bitshift will be 0b000000010
                // when we mask it, we get 0b000000000, which outputs false
                
                if (pressed != was_pressed) { // checks if was pressed and pressed differ, meaning teh key state just changed
                    debounce_time[key_index] = now;
                    if (pressed) {
                        new_state |= (1 << key_index /* creates pattern of key index*/); // uses the OR operator, which does not change the 1 of 1 but adds anotehr 1
                    }
                } else if (pressed) { // checks if key was pressed but its the same as was pressed
                    new_state |= (1 << key_index); // this is here for sustained presses
                }
            } else {
                if((current_state >> key_index) & 1 /* tests if the current state is the key index*/) {
                    new state |= (1 << key_index); // this is the else case so the 5ms has not elapsed so we just set it to the current state
                }
            }

            // this logic is confusing, so here's an example
            // you press key 4:
            // if t=0ms, case 1 triggers, which sets the bit and starts th etimer
            // if t = 10ms, then case 2 triggers, which maintians teh bit
            // if t = 3ms, then case 3 triggers, which maintains the bit
            // state 3 exists such that bouncing does not corrupt

        }

    }
    current_state = new_state; // sets the new current state to the new state
    return current_state;
}