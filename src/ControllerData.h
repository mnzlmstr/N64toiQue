#ifndef CONTROLLER_DATA_H
#define CONTROLLER_DATA_H

struct ControllerData
{
  bool buttonA;
  bool buttonB;
  bool buttonZ;
  bool buttonL;
  bool buttonR;
  bool buttonStart;

  bool DPadUp;
  bool DPadDown;
  bool DPadLeft;
  bool DPadRight;

  bool CUp;
  bool CDown;
  bool CLeft;
  bool CRight;

  short xAxis;
  short yAxis;

  bool xAxisRaw[8];
  bool yAxisRaw[8];

};

#endif 