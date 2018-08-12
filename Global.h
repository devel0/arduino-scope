#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <iostream>
#include "AdcEval.h"

class Global
{
private:
  static Global *instance;

public:  
  Global();

  static Global &Instance();
  int OpenSerial(const char *device, int rate);
};

#endif
