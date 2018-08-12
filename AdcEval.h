#ifndef _ADC_EVAL_H
#define _ADC_EVAL_H

class ADCEval
{

private:
  int adcValueMin;
  double voltageMin;
  int adcValueMax;
  double voltageMax;

public:  
  ADCEval(int adcValueMin, double voltageMin, int adcValueMax, double voltageMax);

  double GetVoltage(int adc);
  int GetADC(double voltage);
};

#endif