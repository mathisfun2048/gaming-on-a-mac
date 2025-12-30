#include "usb_hid.h"
#include "pico/stdlib.h"
#include "tusb.h" // this is in cmake, this interface imports tusb_config that we wrote earlier

void usb_hid_init(void) {
    tusb_init();
}
// initializes usb hid by using the hid driver in tinyusb

void usb_hid_send_key(uint8_t keycode, uint8_t modifers) { // the keycode is as expected, but the modifier is a shift bitmask
    if(!tud_hid_ready()) return; // if HID endpoint istn busy, exit

    uint_t report[8] = {0}; // creates an 8 byte keyboard input
    report[0] = modifers; // first byte is the modifier bitmap
    report[2] = keycode; // we don't put keycode in slot 1 bc that is reserved by HID specifications

    /*
    here's teh report structure:
    0. modifiers
    1. reserved for HID
    2. key 1
    3. key 2
    4. key 3
    5. key 4
    6. key 5
    7. key 6

    this structure means we are limited to 6 keys pressed at the same time
    (this prob wont happen)
    ((and if it does, we should prob not do anything))
    */

    tud_hid_keyboard_report(1, modifiers, &report[2]); // sends input with tinyusb: first one is the report ID, modifiers, and pointer to the 6-keycode array
    sleep_ms(10); // waits for that report to be sent
}

void usb_hid_send_consumer(uint16_t code) {
    if(!tud_hid_ready()) return; // same thing as above, if its not ready, exit

    tud_hid_report(2, &code, sizeof(code)); .// npw sends reprot using second interface, &code is teh pointer to teh media keycode, the last is teh size of teh data
    sleep_ms(10); // waits for that report to be sent

    uint16_t zero = 0;
    tud(hid)report(2, &zero, size_of(zero)); // releases that key
}

void usb_hid_release_all(void) {
    if(!tud_hid_ready()) return;

    uint8_t report[8] = {0};
    tud_hid_keyboard_report(1, 0, &report[2]); // basically a copy of usb_hid_send_key, but clears it instead; making one seperate adds clarity
}

extern uint8_t const desc_hid_report[]; // extern means taht this var exists in another file (its in usb_descriptors.c)

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) { // this is teh callback when the USB is enumerated
    return desc_hid_report;
}
// the star means it returns a pointer bc C arrays can only be pointed to

// the next 2 functions are just so that tinyusb dosn't get mad

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reglen) {
    return 0;
}
// host might ask for keybaord state, but we don't really need that so we return no data

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
}
// sends data to device (normally led states), we don't have leds, so no data

/* for funs and giggles, here's waht each of the inputs mean (if you choose to make something that does need feedback or have leds:)

uint8_t instance -> which HID interface
uint8_t report_id -> which HID interface, again (but add 1)
hid_report_type_t is a enum defined by tinyusb
-> HID_REPORT_TYPE_INPUT: device -> host
-> HID_REPORT_TYPE_OUTPUT: host -> device
-> HID_REPORT_TYPE_FEATURE: device <-> host
the buffer vars just point to the buffer we defined and how big it is

*/