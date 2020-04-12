#ifndef PIN_MAPPINGS_H
#define PIN_MAPPINGS_H

#define DATA_PIN 13

#define PIN_BUTTON_A   15
#define PIN_BUTTON_B   5
#define PIN_BUTTON_Z   23
#define PIN_BUTTON_S   1
#define PIN_BUTTON_L   1
#define PIN_BUTTON_R   1

#define PIN_DPAD_UP    1
#define PIN_DPAD_DOWN  1
#define PIN_DPAD_LEFT  1
#define PIN_DPAD_RIGHT 1

#define PIN_C_UP       1
#define PIN_C_DOWN     1
#define PIN_C_LEFT     1
#define PIN_C_RIGHT    1

#define PIN_A_AXIS_X   14
#define PIN_B_AXIS_X   27

#define PIN_A_AXIS_Y   2
#define PIN_B_AXIS_Y   4

#define LINE_WRITE_HIGH pinMode(DATA_PIN,INPUT_PULLUP)
#define LINE_WRITE_LOW  pinMode(DATA_PIN,OUTPUT)


#endif 