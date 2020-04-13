#include "Output.h"
#include "PinMappings.h"

#include <Arduino.h>

int encoderXpos = 0;
int encoderYpos = 0;

void setupIO()
{
    // the controller data line 
    LINE_WRITE_HIGH;

    pinMode(PIN_BUTTON_A,OUTPUT);
    pinMode(PIN_BUTTON_B,OUTPUT);
    pinMode(PIN_BUTTON_Z,OUTPUT);
    pinMode(PIN_BUTTON_S,OUTPUT);
    pinMode(PIN_BUTTON_L,OUTPUT);
    pinMode(PIN_BUTTON_R,OUTPUT);

    pinMode(PIN_DPAD_UP,   OUTPUT);
    pinMode(PIN_DPAD_DOWN, OUTPUT);
    pinMode(PIN_DPAD_LEFT, OUTPUT);
    pinMode(PIN_DPAD_RIGHT,OUTPUT);

    pinMode(PIN_C_UP,   OUTPUT);
    pinMode(PIN_C_DOWN, OUTPUT);
    pinMode(PIN_C_LEFT, OUTPUT);
    pinMode(PIN_C_RIGHT,OUTPUT);

    pinMode(PIN_A_AXIS_X, OUTPUT);
    pinMode(PIN_B_AXIS_X, OUTPUT);

    pinMode(PIN_A_AXIS_Y, OUTPUT);
    pinMode(PIN_B_AXIS_Y, OUTPUT);
}

/** Function to simulate the 90° phaseshifted quadrature encoding used by 
 *  the N64-Controller's joystick's linear encoders. As only change in position
 *  (not absolute position) is reported the change in the value between writes 
 *  is needed for calculation. Must be run from RAM to mitigate timing differences in
 *  ESP32's SPI-Flash Chip Access Times. 
 *  @param relativeMovement should be processed by calling 'mapJoystickToEncoderPos'
 *  with the read joystick position.
 */
void IRAM_ATTR moveAxis(int axisPinA,int axisPinB,int requestedEncoderPos,int *realEncoderPos)
{
    // get difference from requested ender position to real encoder position 
    int difference = requestedEncoderPos - *realEncoderPos;

    // store new real encoder position 
    *realEncoderPos = requestedEncoderPos;

    // set pins used for outputting 
    int pinA = axisPinA;
    int pinB = axisPinB;

    // invert pins if moving in the other direction 
    if(difference > 0)
    {
        pinA = axisPinB;
        pinB = axisPinA;
    }

    // get absolute movement value 
    difference = abs(difference);

    // for each change in movement 
    for(int i = 0;i < difference;i++)
    {
        // replicate 90° phaseshifted quadrate output 
        digitalWrite(pinA,LOW);
        delayMicroseconds(2);
        digitalWrite(pinB,LOW);
        delayMicroseconds(6);
        digitalWrite(pinA,HIGH);
        delayMicroseconds(2);
        digitalWrite(pinB,HIGH);

        // debounce as N64 will not detect change correctly otherwise 
        delayMicroseconds(40);
    }
}

/** Function to map Joystick position to relative Encoder Position.
 *  According to Nintendo the maximum value for any axis to be assumed should 
 *  be +- 60. The encoder starts acting strange after ~ 30 turns in a given 
 *  direction. Also passing on ControllerData struct so that user specified
 *  modifiers can be used to specify alternate mapping, eg. for 
 *  easier ESS-Position in Ocarina of Time (see my example)
 */
int mapJoystickToEncoderPos(int joyval,ControllerData *data)
{
    // ESS-Modifier for Ocarina of Time, hold A+B to stay in ESS-Position
    if(data->buttonA && data->buttonB) return -2;

    // deadzone handling 
    if(abs(joyval) < 3) return 0;

    // normally return half ??? 
    return (int)(joyval * .5f);
}

void outputToiQue(ControllerData *data)
{
    // ===== Write Button Data =====
    digitalWrite(PIN_BUTTON_A,data->buttonA);
    digitalWrite(PIN_BUTTON_B,data->buttonB);
    digitalWrite(PIN_BUTTON_L,data->buttonL);
    digitalWrite(PIN_BUTTON_R,data->buttonR);
    digitalWrite(PIN_BUTTON_Z,data->buttonZ);
    digitalWrite(PIN_BUTTON_S,data->buttonStart);

    // ===== Write D-Pad Data =====
    digitalWrite(PIN_DPAD_UP,   data->DPadUp);
    digitalWrite(PIN_DPAD_DOWN, data->DPadDown);
    digitalWrite(PIN_DPAD_LEFT, data->DPadLeft);
    digitalWrite(PIN_DPAD_RIGHT,data->DPadRight);

    // ===== Write C-Button Data =====
    digitalWrite(PIN_C_UP,    data->CUp);
    digitalWrite(PIN_C_DOWN,  data->CDown);
    digitalWrite(PIN_C_LEFT,  data->CLeft);
    digitalWrite(PIN_C_RIGHT, data->CRight);

    // ===== Write Joystick Data =====

    // calculate change in axis between writes 
    //int xAxisChange = data->xAxis - lastxAxis;
    //int yAxisChange = data->yAxis - lastyAxis;

    // move axis accordingly 
    moveAxis(PIN_A_AXIS_X,PIN_B_AXIS_X,mapJoystickToEncoderPos(data->xAxis,data),&encoderXpos);
    moveAxis(PIN_A_AXIS_Y,PIN_B_AXIS_Y,mapJoystickToEncoderPos(data->yAxis,data),&encoderYpos);
}