#ifndef CONTROLLER_BUFFER_H
#define CONTROLLER_BUFFER_H

#include "ControllerData.h"
#include <Arduino.h>

#define DATA_SIZE 450  // number of sample points to poll 
#define DATA_OFFSET 0  // number of samples to ignore after staring to poll 

#define MAX_INCLINE_AXIS_X 60
#define MAX_INCLINE_AXIS_Y 60

#define BIT_THRESHOLD 6

extern void IRAM_ATTR sendCommand(byte command);
extern void populateControllerStruct(ControllerData *data);

/** Function to read the offsets of the individual bits so that they can be updated for reading.
 *  Resolution is detected by taking the distance of two falling edges of Button A and B.
 *  Buttons must not be pressed by the time this command is invoked !
 *  sendCommand(0x01) must be invoked before !
 */
extern void updateOffsetsAndResolution();


#endif 