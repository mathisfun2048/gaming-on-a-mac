#ifndef USB_HID_H
#define USB_HID_H
#include <stdint.h>

// keys we press:
// common to both: space, escape, left shift, and tab
// arrows: up, down, left, right
// wasd: w, a, s, d
#define HID_KEY_W 0x1A
#define HID_KEY_A 0x04
#define HID_KEY_S 0x16
#define HID_KEY_D 0x07
#define HID_KEY_UP_ARROW 0x52
#define HID_KEY_LEFT_ARROW 0x50
#define HID_KEY_DOWN_ARROW 0x51
#define HID_KEY_RIGHT_ARROW 0x4F
#define HID_KEY_SPACE 0x2C
#define HID_KEY_ESCAPE 0x29
#define HID_KEY_LSHIFT 0xE1
#define HID_KEY_TAB 0x2B
 
#define HID_MOD_LSHIFT 0x02
// above are the mappings found in the USB HID manuel

void usb_hid_init(void); // initializes USB HID

void usb_hid_send_key(uint8_t keycode, uint8_t modifers); // sends key
void usb_hid_send_consumer(uint16_t code); // send consumer driver code
void usb_hid_release_all(void); // release all keys

// there are more functions in usb_hid.c, but thsoe are for tinyusb, so we don't need to include them here


#endif