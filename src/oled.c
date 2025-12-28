#include "oled.h"
#include "config.h"
#include "pico/stdlib.h"
#include <string.h>

// for my logic I want 2 layers with arrow keys, space, escape, shift, and tab
// U, D, L, R, W, S, A, D
// SH, SP, E, T
// "ARROW MODE", "WASD MODE"
// -> total list of keys needed:
// " ", W, A, S, D, U, D, L, R, S, M, O, E

// so i'm going to hardcode these values as pixels because eh
// i'll have the dimensions be 5x7: 5 pixels wide, 7 pixels tall

satic const uint8_t [][5]{
    {0x00, 0x00, 0x00, 0x00, 0x00} // this is space! This has 5 cols with nothing! 
    {0x3F, 0x40, 0x38, 0x40, 0x3F} // W
    {} // A
    {} // S
    {} // D
    {} // U
    {} // L
    {} // R
    {} // S
    {} // M
    {} // O
    [] // E

}
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


*/