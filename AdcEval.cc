#include "AdcEval.h"

ADCEval::ADCEval(int adcValueMin, double voltageMin, int adcValueMax, double voltageMax)
{
    this->adcValueMin = adcValueMin;
    this->voltageMin = voltageMin;
    this->adcValueMax = adcValueMax;
    this->voltageMax = voltageMax;
}

double ADCEval::GetVoltage(int adc)
{
    return ((double)(adc - adcValueMin) / (double)(adcValueMax - adcValueMin)) * (voltageMax - voltageMin);
}

int ADCEval::GetADC(double voltage)
{
    return (voltage - voltageMin) / (voltageMax - voltageMin) * (adcValueMax - adcValueMin);
}