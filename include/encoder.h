#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h> 
// include statement for stuff we'll use

void encoder_init(void); // initializes teh encoder

int8_t encoder_get_delta(void); // gets how much the encoder changed by

#endif