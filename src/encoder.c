#include "encoder.h"
#include "config.h"
#include "pico/stdlib.h"

static volatile int8_t encoder_delta = 0;
// volatile means that this variable will not be cached and will be checked/read
// note that this is a signed int, meaning it can be negative!

static volatile uint8_t encoder_state = 0;
// a uint is an unsinged int

static constant int8_t encoder_table[16] = { 
    0, -1, 1, 0,
    1, 0, 0, -1,
    -1, 0, 0, 1,
    0, 1, -1, 0


}
// encoders have 2 terminals, A and B, and thier state can be given by a tuple of the A and B state
// possible states: 00 (low low), 01 (low high), 10 (high low), 11 (high high)
// with these, we find a direction from old to new
// lets make a 4x4 matrix where the header for the rows are the new states and the headers for the cols are the old staets
// now we can see the difference between them to construct our table
// for encoders, if A changes first, that means positive which means clock wise
// if B changes first, that means negative which menas counter clockwise
// both pins cannot change at the same time, so if that signal is read, then that is a wrong signal and we return 0 accordingly

static void encoder_callback(uint gpio, uint32_t events) {
    uint8_t a = gpio_get(ENCODER_A_PIN);
    uint8_t b = gpio_get(ENCODER_B_PIN);
    
    encoder state = ((encoder_state << 2) /* shifts old AB to pits 3:2*/ | (a << 1) /* adds current A to slot 1*/| b /* adds current B to slot 0*/ */) & 0x0F /* keeps only the 4 lower bits*/
    encoder_delta += encoder_table[encoder_state]; // encoder state now has an [old][new] format. Bc the way we set up the encoder table, when we find the value at index [old][new], we get the rotation


}

void encoder_init(void) {
    gpio_init(ENCODER_A_PIN);
    gpio_init(ENCODER_B_PIN);
    gpio_set_dir(ENCODER_A_PIN, GPIO_IN);
    gpio_set_dir(ENCODER_B_PIN, GPIO_IN);
    gpio_pull_up(ENCODER_A_PIN); // pull up resistors are like the pull down resistors we used int eh matrix, but now its a weak connection to 3V3 instead of GND
    gpio_pull_up(ENCODER_B_PIN); // the reason we have these is such that the pin normally reads high, and then the pin can track the change in voltage when the encoder connects to ground

    gpio_set_irq_enabled_with_callback(ENCODER_A_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_callback); // calls callabck whenever voltage changes for A
    gpio_set_irq_enabled(ENCODER_B_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true); // samething as above, don't need to point at function bc above method sets it globally

}

int8_t encoder_get_delta(void) {
    int8_t delta = encoder_delta / 4; // each raw transitionc reates 4 states, so diviidng by 4 give sus the number of clikcs
    encoder_delta -= delta * 4; // not nessicarily 0 bc integer division
    return delta; // returns teh delta
}