#ifndef UPDATER_H
#define UPDATER_H

#include "ControllerBuffer.h"

/** Function to check if the user wants to update the device.
 *  This is signaled by pressing all C Buttons + Start
 *  if so update process is initiated 
 *  @param controller pointer to struct containing presed buttons 
 *  @return none 
 */
extern void checkUpdateCombo(ControllerData *controller);

#endif