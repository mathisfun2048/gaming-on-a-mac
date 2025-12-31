#include "config.h"
#include "usb_hid.h"

#define USB_VID 0x2E8A // vendor assigned to rpi by usb-if
#define USB_PID 0x1001 // rando number you get to pick
#define USB_BCD 0x200 // this menas usb 2.0

uint8_t const desc_hid_report[] {
    TUD_HID_REPRT_DESC_KEYBOARD(HID_REPORT_ID(1)),
    TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(2))
};
// array contianing teh hid reports

tusb_desc_device_t const desc_device = {
    .blegth = sizeof(tusb_desc_device_t), // descriptor size 
    .bDescriptorType = TUSB_DESC_DEVICE, // type identiifer
    .bcdUSB = USB_BCD, // usb spec
    .bDeviceClass = 0x00, // class code
    .bDeviceSubClass = 0x00, // device subclass (unused)
    .bDeviceProtocol = 0x00, // protocol (unused)
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE, // EP0 max packet
    .idVendor = USB_VID, // vendor ID for rpi
    .idProduct = USB_PID, // product ID (defined earlier, custom for this device)
    .bcdDevice = 0x0100, // this is device version 1
    .iManufacturer = 0x01, //  index for manufacturer name
    .iProduct = 0x02, // index for product name
    .iSerialNumber = 0x03, // index foer serial number
    .bNumConfigerations = 0x01 // number of configurations = 1
}; // this is read during enumeration 

uint8_t const * tud_descriptor_device_cb(void) {
    return (uint8_t const *) &desc_device; // returns pointer to desc_device
}

enum {
    ITF_NUM_HID, 
    ITF_NUM_TOTAL
};
// config for tusbkl

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN) // defines teh total length to be the config length plus hid lenght

uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), 0x81, CFG_TUD_HID_EP_BUFSIZE, 1)
};

/* okay this is long because it took me a bit to understand what this is

This arrya containes teh config header + HID interface descriptor concatenated 

lets go into teh parameters of the functions:
TUD_CONFIG_DESCRIPTOR:
1 -> confg number
ITF_NUM_TOTAL -> total interface count
0 -> string descriptor index (0 means no string bc we dont have one)
CONFIG_TOTAL_LEN -> total descriptor length in bytes (we calcualted this earlier!)
TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP -> atributes a bitmask: either always on, self powered, or remote wakeup
100 -> maximum current is 200mA

TUD_HID_DESCRIPTOR:
ITF_NUM_HID -> interface number 
0 -> string descriptor index, again 0 bc no string
HID_ITF_PROTOCOL_NONE -> rport protocol only
sizeof(desc_hid_report) -> defines report format
0x81 -> endpoint address
CFG_TUD_HID_EP_BUFSIZE -> endpoint buffer size
1 -> poll every 1ms

*/

uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    return desc_configuration; // from my understanding, this is just to make tinyusb happy, we dont need to inorporate index into the result bc our project is decently basic
}

char const* string_desc_arr[] = {
    (const char[]) {0x09, 0x04}, // project is in english
    "Arya C.", // manufacturer
    "gaming on a mac", // product
    "000001" // serial
};

static uint16_t _desc_str[32];
// this is a static buffer. we are using 16 bits becasue USB strings use UTF-16LE. 

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) { // callback when host wants a string descriptor, index gives all teh metadata, and langid gives teh language id

    uint8_t chr_count; // charachter counter for string length

    if(index == 0) {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);  // copy 2-byte language code to position 1, position 0 still reserved for descriptor header
        chr_count = 1; // language descriptor just has 1 language
        // if index 0, then return supported language codes
    } else {
        if(index >= sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) { // bounds checks if index is out of range
            return NULL;
        }

        const char* str = string_desc_arr[index]; // gets ASCII string pointer from array

        chr_count = strlen(str); // charachter count is the number of charachters in teh string

        if(chr_count > 31) {
            chr_count = 31; // chr count must be < 32 to prevent buffer overflow
        }

        for(uint8_t i = 0; i < chr_count; i++) {

            _desc_str[1+i] = str[i]; 
            // converts ASCII to UTF-16LE, starts at position 1 bc position 0 is teh header

        }
        
    }

    _desc_str[0] = (TUSB_DESC_STRING << 8) /* this is the upper byte */ | (2*chr_count + 2) /* this is the lower byte*/; 

    return _desc_str;


}
// we use UTF-16LE encoding because USB sepcification mandates uicode and not ascii
// we use a static buffer to reuse memory
// index 0 has to be descp because of USB specs


