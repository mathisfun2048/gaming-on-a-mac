#ifndef TUSB_CONFIG_H
#define TUSB_CONFIG_H

/* 
okay yap time on what this is: tiny usb

tinyusb is a usb device stack that implements USB protocols 

components:
- application layer: main application code
- class drivers: implmenet usb device classes
- usb core: wraps USB packet with endpoint address, timing sequences; manages enumuration
- controller device: writes to RP2040's USB registers and triggers DMA transfers
- hardware :convert to electric signals 

-> tinyusb handles most of this complexity though

what is enumuration?
- when you plug this in, the RP2030 pulls teh D+ line hihg
- the host sends reset causing teh RP2040 to enter teh defult state
- host requests device description. via endpoint 0
- host assings unique usb address
- host reads description and selects a configuration
- HID drivers initialize

*/

#ifdef __cplusplus // chekcs if compiling as C++
extern "C" { // tels c++ to use C-style function naming
#endif

#define CFG_TUSB_RHPORT0_MODE OPT_MODE_DEVICE
// this configures USB port 0 as device mode -> this allows the RP2040 to act as a USB device

#ifndef CFG_TUSB_DEBUG
#define CFG_TUSB_DEBUG 0 // disables tinyusb's debug output
#endif

#define CFG_TUD_ENDPOINT0_SIZE 64 // sets control endpoint 0 buffer to 64 bytes
// endpoint 0 handles USB enumeration and config, and 64 bytes is the maximum for full speed USB


#define CFG_TUD_HID 2 // enables 2 hid interfaces: interface 0: keystrokes, interface 1: consumer controls (up, down, left, right)
// each of these 2 interfaces gets its own endpoint, and tinyusb allocates them automatically :p
#define CFG_TUD_CDC 0 // disables virtual cerial port 
#define CFG_TUD_MSC 0 // disables mass storage (usb drive emulation)
#define CFG_TUD_MIDI 0 // disables midi
#define CFG_TUD_VENDOR 0 // disables custom protocols

#define CFG_TUD_HID_EP_BUFSIZE 16 // 16 bytes to accomodate reports from our 2 interfaces

#ifdef __cplusplus // technichally we didn't need to do this bc our project is in C, but its good practice to incldue this
}
#endif

#endif