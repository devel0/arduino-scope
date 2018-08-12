#include "Global.h"

#include "AdcEval.h"

#include <iostream>

const int ADC_VALUE_MIN = 0;
const int ADC_VALUE_MAX = 1023;

const double VOLTAGE_MIN = 0.0;
const double VOLTAGE_MAX = 5.0;

Global *Global::instance = NULL;

Global::Global() : adcEval(*(new ADCEval(ADC_VALUE_MIN, VOLTAGE_MIN, ADC_VALUE_MAX, VOLTAGE_MAX)))
{    

}

Global &Global::Instance()
{
    if (instance == NULL)
    {
        instance = new Global();                
    }

    return *instance;
}