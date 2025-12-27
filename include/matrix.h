#ifndef MATRIX_H // checks "hey does this exist?"
#define MATRIX_H // if no, make it!

#include <stdint.h> // this includes standard integer types! This ensures consistancy 
// wait this is actually interesting
// int sizes vary based on platform! importing this allows us to use 8-bits for int!

#include <stdbool.h> // this imports the bool type and allows for true/false keywords

void matrix_int(void); // this initializes the matrix

uint16_t matrix_scan(void); // this scans the matrix and returns a bitmask of which key is pressed
// a bitmask is just a 16 bit integer whose position with a 1 is the key pressed



#endif // closes if statement
