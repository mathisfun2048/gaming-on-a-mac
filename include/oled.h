#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include <stdbool.h>

#define OLED_WIDTH 128 // this is the num pixels wide my oled is
#define OLED_HEIGHT 32 // num pizels tall
#define OLED_ADDRESS 0x3C // we are going to be using I2C and this is teh defult address

bool oled_init(void); // initializes teh oled

void oled_clear(void); // sets everything to zero

void oled_text(uint8_t x, uint8_t y, const char* str); // displays text at col x and page y

void oled_update(void); // updates the oled

void oled_draw_grid(const char* keys[9], const char* title); // draws on the oled

#endif