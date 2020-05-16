#ifndef PIN_MAPPINGS_H
#define PIN_MAPPINGS_H

#define DATA_PIN 13

#define PIN_BUTTON_A   15
#define PIN_BUTTON_B   5
#define PIN_BUTTON_Z   32
#define PIN_BUTTON_S   23
#define PIN_BUTTON_L   33
#define PIN_BUTTON_R   17

#define PIN_DPAD_UP    26
#define PIN_DPAD_DOWN  16
#define PIN_DPAD_LEFT  25
#define PIN_DPAD_RIGHT 18

#define PIN_C_UP       22
#define PIN_C_DOWN     21
#define PIN_C_LEFT     19
#define PIN_C_RIGHT    12

#define PIN_A_AXIS_X   14
#define PIN_B_AXIS_X   27

#define PIN_A_AXIS_Y   4
#define PIN_B_AXIS_Y   2

#define LINE_WRITE_HIGH pinMode(DATA_PIN,INPUT_PULLUP)
#define LINE_WRITE_LOW  pinMode(DATA_PIN,OUTPUT)


#endif 
