#ifndef _SIGNAL_STAT_H
#define _SIGNAL_STAT_H

#include <chrono>

#include "Global.h"

#define ADC_VALUES_SIZE 512

class SignalStat
{

private:
  double Vmin;
  double Vmax;
  double Vppk;
  double VminThresHold;
  double VmaxThresHold;
  Global &global;
  double adcValues[ADC_VALUES_SIZE];
  std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds> refTimestamp;
  int adcValueCount = 0;
  int adcValuePos = 0;

  long int freqCnt = 0;
  bool freqIsTop = false;
  double freq;

public:
  SignalStat();
  void Reset();
  void AddAdcValue(int adcValue);

  double GetVmin();
  double GetVmax();
  double GetVppk();
  double GetFreq();
  double GetVminThresHold();
  double GetVmaxThresHold();
};

#endif