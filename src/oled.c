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

satic const uint8_t [][5]{
    {0x00, 0x00, 0x00, 0x00, 0x00} // this is space! This has 5 cols with nothing! 
    {0x3F, 0x40, 0x38, 0x40, 0x3F} // W
    {0x7E, 0x11, 0x11, 0x11, 0x7E} // A
    {0x46, 0x49, 0x49, 0x49, 0x31} // S
    {0x7F, 0x41, 0x41, 0x22, 0x1C} // D
    {0x3F, 0x40, 0x40, 0x40, 0x3F} // U
    {0x7F, 0x40, 0x40, 0x40, 0x40} // L
    {0x7F, 0x09, 0x19, 0x29, 0x46} // R
    {0x7F, 0x08, 0x08, 0x08, 0x7F} // H
    {0x7F, 0x09, 0x09, 0x09, 0x06} // P
    {0x7F, 0x49, 0x49, 0x49, 0x41} // E
    {0x01, 0x01, 0x7F, 0x01, 0x01} // T
    {0x3E, 0x41, 0x41, 0x41, 0x3E} // O
    {0x7F, 0x02, 0x0C, 0x02, 0x7F} // M

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