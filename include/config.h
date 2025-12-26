// the purpous of thsi document is to define hardware pin mapping
// this document is when it starts getting fun because its C and not CMake :)

#ifndef CONFIG_H // checks if macro does not exist
#define CONFIG_H // starts defining the macro

#define NUM_ROWS 3
#define NUM_COLS 3
/*
for my switch matrix I chose to implement a row-col system


row 1 -> 1 2 3
row 2 -> 4 5 6
row 3 -> 7 8 9

<cols not pictured becasue typing that is hard>
*/
#define DEBOUNCE_MS 5
// just defines how much time we expect the debouce of keys to be

#define COL1_PIN 0
#define COL2_PIN 1
#define COL3_PIN 2
// this indicates which gpios my col lines are connected to
// make sure you do teh GPIO pins, not the pin number

#define ROW1_PIN 3
#define ROW2_PIN 4
#define ROW3_PIN 6
// same thing as above
// theres a jump between 4 and 6 bc my mcu does not have a gpio 5 haha

#define ENCODER_A_PIN 26
#define ENCODER_B_PIN 27
// same thing but now for encoder pins

#define OLED_SCL_PIN 29
#define OLED_SDA_PIN 7
// same thing as before
// I cannot use defult SDA and SCL because I wired my pcb incorectly 
// so we will implement software I2C in this project

#define KEY_TOGGLE 7
// I plan to have multiple layers of up/down/left/right depending on game, and we need a toggle between those layers
// this makes index 7, key 8 (row 3 middle) to be the key that toggles!

#endif
// this closes the if statement we had in the begining
