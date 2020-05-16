#include "Output.h"
#include "ControllerData.h"
#include "ControllerBuffer.h"
#include "Debug.h"
#include "Updater.h"

ControllerData controller;

void setup()
{
  Serial.begin(115200);

  // setup io pins 
  setupIO();

  #ifdef PLOT_CONSOLE_POLLING
  delay(5000);
  sendCommand(0x01);
  while(true);
  #endif 

  sendCommand(0x01);
  updateOffsetsAndResolution();
}


void loop()
{
  // send command 0x01 to n64 controller 
  sendCommand(0x01);

  // store received data in controller struct 
  populateControllerStruct(&controller);

  // output received data to ique 
  outputToiQue(&controller);

  // polling must not occur faster than every 20 ms 
  delay(14);

  checkUpdateCombo(&controller);

  //Serial.printf("DPAD: %i %i %i %i \n",controller.DPadUp,controller.DPadDown,controller.DPadLeft,controller.DPadRight);
  //Serial.printf("C: %i %i %i %i \n",controller.CUp,controller.CDown,controller.CLeft,controller.CRight);

  //delay(5000);
}
