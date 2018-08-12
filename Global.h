#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <iostream>
#include "AdcEval.h"

class Global
{
private:
  static Global *instance;

public:
  ADCEval& adcEval;

  Global();

  static Global &Instance();
};

#endif
