#include "pico/stdlib.h"
#include "tusb.h"
#include "usb_hid.h"
#include "config.h"
#include "matrix.h"
#include "encoder.h"
#include "oled.h" 

typedef enum {
    LAYER_WASD = 0,
    LAYER_ARROWS = 1
} layer_t;
// defines enums for our 2 layers

static layer_t current_layer = LAYER_WASD; // sets basic layer to wasd


// i realized the macropad looks better when rotated 90 degrees CCW, so this is the matrix that trasforms from teh physical layout to logical layout

/*
Here's a visualization:
Logical:
[0, 1, 2]
[3, 4, 5]
[6, 7, 8]

Physical:
[2, 5, 8]
[1, 4, 7]
[0, 3, 6]

we map from physical to logical in this array


*/
static const uint8_t key_rotation[9] = {
    6, 3, 0, 7, 4, 1, 8, 5, 2
}

static const struct {
    uint8_t keycode;
    uint8_t modifiers;
} layer_wasd[9] = {
    {HID_KEY_ESCAPE, 0},
    {HID_KEY_W, 0},
    {HID_KEY_TAB, 0}, 
    {HID_KEY_A, 0},
    {HID_KEY_S, 0}, 
    {HID_KEY_D, 0}, 
    {HID_KEY_LSHIFT, 0}, 
    {0,0} // toggle
    {HID_KEY_SPACE, 0}
}
// lookup table for wasd whose logical index returns a keycode and modifer

static const struct {
    uint8_t keycode;
    uint8_t modifiers;
} layer_arrows[9] = {
    {HID_KEY_ESCAPE, 0}, 
    {HID_KEY_UP_ARROW, 0}, 
    {HID_KEY_TAB, 0}, 
    {HID_KEY_LEFT_ARROW, 0}, 
    {HID_KEY_DOWN_ARROW, 0}, 
    {HID_KEY_RIGHT_ARROW, 0}, 
    {HID_KEY_LSHIFT, 0}, 
    {0,0}, 
    {HID_KEY_SPACE, 0}
}
// lookup table for arrow layer

static const char* wasd_lables[9] = {
    "E", "W", "T",
    "A", "S", "D",
    "SH", "", "SP"
};
// visual represntation on how things should look for teh wasd table

static const char* arrows_lables[9] {
    "E", "U", "T",
    "L", "D", "R",
    "SH", "", "SP"
}
// same thing but now for arrows

void update_display(void) {
    if(current_layer == LAYER_WASD) {
        oled_draw_grid(wasd_lables, "WASD MODE");
    } else {
        oled_draw_grid(arrow_lables, "ARROW MODE")
    }
}
// draws the oled!

void handle_key_press(uint8_t key_index) {
    if(key_index == KEY_TOGGLE) {
        current_layer = (current_layer == LAYER_WASD) ? LAYER_ARROWS : LAYER_WASD;
        update_display();
        return;
    }

    if(current_layer = LAYER_WASD) {
        usb_hid_send_key(layer_wasd[key_index].keycode, layer_wasd[key_index].modifiers);
    } else {
        usb_hid_send_key(layer_arrows[key_index].keycode, layer_arrows[key_index].modifiers);
    }
}
// gets keypress and sends it through tinyusb


void handle_encoder (int8_t delta) {
    if(delta == 0) {
        return;
    } else if(delta > 0) {
        usb_hid_send_key(HID_KEY_RIGHT_ARROW, 0);
        usb_hid_release_all();
    } else {
        usb_hid_send_key(HID_KEY_LEFT_ARROW, 0);
        usb_hid_release_all();
    }
}
// implements encoder as arrow keys

int main() {
    stdio_init_all(); // initializes hardware
    sleep_ms(1000); // wait for usb to enumurate

    matrix_init();
    encoder_init();
    usb_hid_init();

    if(!oled_init()) { // even calling this in this if loop initilaizes it, so we don't need to initialize again
        // oled failed lol lets see if other things still work
    }

    update_display();

    uint16_t last_matrix_state = 0;

    while(1) {
        tud_task(); // updates USB

        uint16_t matrix_state = matrix_scan(); // scans matrix

        for(uint8_t i = 0; i <= 9; i++) {
            if(matrix_key_pressed(i)) {
                uint8_t logical_key = key_rotation[i]; // applies rotation taht we established earlier
                handle_key_press(logical_key);
            }
            if(matrix_key_released(i)) {
                usb_hid_release_all();
            }
        }
        
        int8_t encoder_delta = encoder_get_delta();
        if(encoder_delta != 0) {
            handle_encoder(encoder_delta);
        }
        sleep_ms(1); // mild sleep
    }

    return 0;
}
