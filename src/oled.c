#include "oled.h"
#include "config.h"
#include "pico/stdlib.h"
#include <string.h>

// for my logic I want 2 layers with arrow keys, space, escape, shift, and tab
// U, D, L, R, W, S, A, D
// SH, SP, E, T
// "ARROW MODE", "WASD MODE"
// -> total list of keys needed:
// " ", W, A, S, D, U, L, R, H, P, E, T, O, M

// so i'm going to hardcode these values as pixels because eh
// i'll have the dimensions be 5x7: 5 pixels wide, 7 pixels tall

satic const uint8_t font5x7[][5]{
    {0x00, 0x00, 0x00, 0x00, 0x00} // " " 0x20 0
    {0x3F, 0x40, 0x38, 0x40, 0x3F} //  W  0x57 1
    {0x7E, 0x11, 0x11, 0x11, 0x7E} //  A  0x41 2
    {0x46, 0x49, 0x49, 0x49, 0x31} //  S  0x53 3
    {0x7F, 0x41, 0x41, 0x22, 0x1C} //  D  0x44 4
    {0x3F, 0x40, 0x40, 0x40, 0x3F} //  U  0x55 5
    {0x7F, 0x40, 0x40, 0x40, 0x40} //  L  0x4C 6
    {0x7F, 0x09, 0x19, 0x29, 0x46} //  R  0x52 7
    {0x7F, 0x08, 0x08, 0x08, 0x7F} //  H  0x48 8
    {0x7F, 0x09, 0x09, 0x09, 0x06} //  P  0x50 9
    {0x7F, 0x49, 0x49, 0x49, 0x41} //  E  0x45 10
    {0x01, 0x01, 0x7F, 0x01, 0x01} //  T  0x54 11
    {0x3E, 0x41, 0x41, 0x41, 0x3E} //  O  0x4F 12
    {0x7F, 0x02, 0x0C, 0x02, 0x7F} //  M  0x4D 13

}

static uint8_t ascii_to_font(uint8_t a) {
    if(a == 0x20) {
        return 0;
    } elif (a == 0x57) {
        return 1;
    } elif (a == 0x41) {
        return 2;
    } elif (a == 0x53) {
        return 3;
    } elif (a == 0x44) {
        return 4;
    } elif (a == 0x55) {
        return 5;
    } elif (a == 0x4C) {
        return 6;
    } elif (a == 0x52) {
        return 7;
    } elif (a == 0x48) {
        return 8;
    } elif (a == 0x50) {
        return 9;
    } elif (a == 0x45) {
        return 10;
    } elif (a == 0x54) {
        return 11
    } elif (a == 0x4F) {
        return 12;
    } elif (a == 0x4D) {
        return 13;
    } else {
        return 0; // returns space if char isn't defined
    }
}

/* 
hm im going to have to implement a dictionary becasue 
*/

// testing canvas to see how this looks
/*
space:
00000
00000
00000
00000
00000
00000
00000
00000

W:
10001
10001
10001
10101
10101
10101
01010
00000

A:
01110
10001
10001
10001
11111
10001
10001
00000

S:
01111
10000
10000
01110
00001
00001
11110
00000

D:
11100
10010
10001
10001
10001
10010
11100
00000

U:
10001
10001
10001
10001
10001
10001
01110
00000

L:
10000
10000
10000
10000
10000
10000
11111
00000

R:
11110
10001
10001
11110
10100
10010
10001
00000

H:
10001
10001
10001
11111
10001
10001
10001
00000

P:
11110
10001
10001
11110
10000
10000
10000
00000

E:
11111
10000
10000
11110
10000
10000
11111
00000

T:
11111
00100
00100
00100
00100
00100
00100
00000

O:
01110
10001
10001
10001
10001
10001
01110
00000

M:
10001
11011
10101
10101
10001
10001
10001
00000

*/

static uint8_t display_buffer[OLED_HEIGHT / 8][OLED_WIDTH] 
// this speeds up display updates by chunking up the display
// I want the display to look like this:
// <LAYER NAME>
// < first row of controls>
// <second row of controls>
// <third row of controls>
// this in total is 4 layers, which is why we split it up into 4 128x8 chunks


static void i2c_start(void) {
    gpio_set_dir(OLED_SDA_PIN, GPIO_OUT); // sets teh gpio pin to be an output pin
    gpio_put(OLED_SDA_PIN, 1); // enables the sda pin
    gpio_put(OLED_SCL_PIN, 1) // enables the scl pin
    sleep_us(1); // mild delay to make sure that both pins are high 
    gpio_put(OLED_SDA_PIN,0); // SDA being low while SCL is high is the start condition for I2C
    sleep_us(1); // mild delay to make sure I2C has the opportunity to start
    gpio_put(OLED_SCL_PIN, 0) // SCL can now be ready for data, so now its low
    sleep_us(1); // mild setup time
}

static void i2c_stop(void) {
    gpio_set_dir(OLED_SDA_PIN, GPIO_OUT); // enables gpio pin to be an output pin
    gpio_put(OLED_SDA_PIN, 0); // make sure sda is low
    gpio_put(OLED_SCL_PIN, 1); // meakes sure SCL is on
    sleep_us(1); // mild time to make sure above ocnditions are set
    gpio_put(OLED_SDA_PIN, 1); // stop condition for I2C is low to high for SDA while SCL is on
    sleep_us(1); // mild time for it to stop
}

/*
Before I go on, I should prob give a brief overview of what I2C is!
- it stands for Inter-Integrated Circuit that contains 2 serial busses: SDA is data, and SCL is clock
- protocol starts with SDA switching from high to low while SCL is high
- master chips generates the clock and the slave chips respond, in our case the master is the rp2040 and the slave is the oled
- (ik wierd namign convention but ece, ig)
- master sends data to SDA when SCL is low
- slave reads SDA when SCL is high
- for every bit if information sent, the slave sent sends an ACK by pulling the SDA to low
- e.x. when both SCL and SDA are high, that means the slave chip reads the data
- e.x. cont. when the SDA then routes that SDA to ground, that pulls it down, which makes the GPIO read SDA as low, which is the ack
- there's also a control byte that tells if the next bytes are command bytes or pixel data
- e.x 0x00 means "interpret the next byte as a command"
- e.x. 0x40 is a control signifier
- heres the general sequence of information transmitted:
- <start> -> 7-bit address -> ACK -> [Control] -> ACK -> [Data] -> ACK -> ... -> STOP

OLED Specific:
- our OLED contains teh SSD 1360 controller
- it has 256 step brightness controls
- based on our buffer earlier, we have 4 pages 8 pixles tall x 128 cols
- byte is weritten to col N, page P controls pixels at (N, P*8) through (N, P*8+7); bit 0 = top of pixel of 8, bit 7 = bottom of pixel (this is exactly like we coded above!)
- e.x. lets say we wanted to type a letter at position (10, 0) -> this menas at col 10 on the first page
- e.x. cont. then, for each col in teh page, we will add the bits per our font table!

*/

static bool i2c_write_byte(uint8_t byte) {
    gpio_set_dir(OLED_SDA_PIN, GPIO_OUT); // sets SDA to be out
    for (int i = 7; i >= 0; i--) {
        gpio_put(OLED_SDA_PIN, (byte >> i) & 1);
        // this algorithm is pretty cool
        // the for loop iterates between all the indecies of a byte, and we shift the index to the first slot in the byte
        // we then mask it with 1 to see if its true, or false
        // then, we can write to the oled sda if that index is true or false
        // this way, we can read the boolean infromation we stored as numbers

        sleep_us(1); // ensures that whatever state we wanted SDA to be put in, it achieves taht state
        gpio_put(OLED_SCL_PIN, 1); // enables SCL pin which causes slave to read
        sleep_us(1); // mild sleepy such that scl is high now and slave gets a chnace to read
        gpio_put(OLED_SCL_PIN, 0); // low again so we can inpu tmore data
        sleep_us(1); // mild nappy nap
    }

    gpio_set_dir(OLED_SDA_PIN, GPIO_IN);
    gpio_put(OLED_SCL_PIN, 1);
    sleep_us(1);
    bool ack = !glio_get(OLED_SDA_PIN); // if ack sent, then SDA will read low, so we need the opposite of that
    gpio_put(OLED_SCL_PIN, 0);
    sleep_us(1);
    // this above algorihtm checks if an ack message is sent

    return ack;
}

static void oled_command(uint8_t cmd) {
    i2c_start();
    i2c_write_byte(OLED_ADDRESS << 1) // shifting left by one creates a 0 in the 0 bit index -> this means write mode (master -> slave); if this were a 1 instead, then it would be read mode (slave -> master)
    i2c_write_byte(0x00); // this means expect a command
    i2c_write_byte(cmd); // writes teh command
    i2c_stop();
}

static void oled_data(uint8_t data) {
    i2c_start();
    i2c_write_byte[OLED_ADDRESS << 1]
    i2c_write_byte(0x40); // this means expect some data
    i2c_write_byte(data);
    i2c_stop();
}

bool oled_init(void) {
    gpio_init(OLED_SCL_PIN);
    gpio_init(OLED_SDA_PIN);
    gpio_set_dir(OLED_SCL_PIN, GPIO_OUT);
    gpio_set_dir(OLED_SDA_PIN, GPIO_IN);
    gpio_put(OLED_SCL_PIN, 1);
    gpio_put(OLED_SDA_PIN, 1);

    sleep_ms(100); // long sleep

    // initialization 
    oled_command(0xAE); // turns display off
    oled_command(0xD5); // ** initalizes teh clock
    oled_command(0x80); // sets clock to defult frequency with divide ratio 1 
    // the lower 4 bits is the divide ratio. Because this is equal to 0, it means it is the fastest refresh (though consumes more power)
    // teh upper 4 bits is teh oscilater frequency, which is 8 
    oled_command(0xA8); // ** set multiplex ratio command (next value will be multiplex ratio)
    oled_command(0x1F); // we have 32 rows
    oled_command(0xD3); // ** next command is going to set the dispay offset
    oled_command(0x00); // don't offset it! 
    oled_command(0x40); // start line: 0
    oled_command(0x8D); // ** charge pump next command: OLED cannot work wiht the 3v3 supplied, it needs 7-14 volts 
    oled_command(0x14); // sets charge pump to self-powered: we will supply 3v3, the oled will make the 7-14 volts
    oled_command(0x20); // next command is going to show teh memory mode
    oled_command(0x00); // memory mode is going to be horizontal -> this allows us to write down addresses for each col in a page, and then span that out for a page, and when done, wrap to the next page
    oled_command(0xA1); // segment remap: we flip it horizontally (this is because of the physical wiring of my oled)
    oled_command(0xC8); // COM scan direction (flip vertically) -> same reason, flip because of teh physical wiring
    oled_command(0x81); // ** contrast!
    oled_command(0x8F); // sets contrast at 143/255
    oled_command(0xD9); // ** starts pre-charge period: charges OLED capacitors before writing data (oleds are technichally a capacitor)
    oled_command(0xF1); // defines precharge period: 1 discharge time, 15 precharge time -> bright but mildly slow
    oled_command(0xD8); // ** VCOMH deselect: sets voltage for unlit pixels
    oled_command(0x40); // sets VCOMH deselct 
    oled_command(0xA4); // display RAM content display
    oled_command(0xA6); // normal display, not inverted (inverse means taht 0 -> on, 1 -> off) jklklk
    oled_command(0xAF); // turns the display on!

    oled_clear(); // clears the oled (nothing's on)
    return true;
}

void oled_clear(void) {
    memset(display_buffer, 0, sizeof(display_buffer)); // sets the entier buffer to zero
}

void oled_text(uint8_t x, uint8_t y, const char* str) {
    // x is teh starting col [0-127] and y is the page number
    // str points to the first charachter of a string in memory
    // e.x.: "Hi" in memory:
    // e.x. cont: Address: 0x1000: 'H', 0x1001: 'i', 0x1002: '\0'
    // e.x. const char* str points to 0x1000

    if (y >= 4) return; // for our function, we only have 4 pages, so y cannot be 4 or greater

    while (*str && x < OLED_WIDTH - 5) { // first we check if the pointer dosn't point to the null char, and then makes sure that we don't pring in the last 5 cols
        uint8_t c = *str; // gets ascii of charachter at the pointer

        if(c >= 32 && c <= 95) { // checks if the charachter is printable
            for (int i = 0; i < 5; i++) { // we're going to itterate through every collumn

                display_buffer[y][x+i] = font5x7[ascii_to_font(c)][i];
            }
            x += 6 // advances cursor 5 pixels for charachter + 1 pixel for spacing
        }
        str++; // moves pointer to the next charachter
    }
}

void oled_update(void) {
    for(uint8_t page = 0; page < 4; page++) {
        oled_command(0xB0 + page); // sets page num
        oled_command(0x00); // sets col address lower 4 to 0
        oled_command(0x10); // set col adress upper 4 bits to 0
        // these above control which col index we start at

        for(uint8_t col; col < oled_width; col++) { // we itterate through all 128 cols
            oled_data(display_buffer[page][col]); // sends one byte to oled, increments
        }
    }
} 

void oled_draw_grid(const char* keys[9], const char* title) { // the keys array has 9 string pointers to represent our 9 keys, and title for our layer title
    oled_clear(); // sets a blank canvas

    oled_text(0, 0, title); // draws title

    

}