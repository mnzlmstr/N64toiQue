#ifndef OUTPUT_H
#define OUTPUT_H

#include "ControllerData.h"

/** Function to output Data stored in ControllerData struct to the iQue console.
 *  Buttons states are directly set usig digitalWrite(), Axis Data is written by 
 *  Simulating the 90° shifted quadrate encoding used by the N64 joysticks. 
 *  As these operate on a relative basis the difference between the current 
 *  and the last joystick positions are calculated and simulated.
 */
extern void outputToiQue(ControllerData *data);

/** Function to setup the IO used for interfacing with the N64-Controller
 *  and the iQue-Console
 */
extern void setupIO();

#endif