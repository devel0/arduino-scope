#include "SignalStat.h"
#include "Global.h"

#include <chrono>
#include <limits>

using namespace std;

SignalStat::SignalStat() : global(Global::Instance())
{
    refTimestamp = chrono::system_clock::now();
    Reset();
}

void SignalStat::Reset()
{
    Vmin = std::numeric_limits<double>::max();
    Vmax = std::numeric_limits<double>::min();
    Vppk = 0;
}

void SignalStat::AddAdcValue(int adcValue)
{
    auto now = chrono::system_clock::now();
    auto computeFreq = (now - refTimestamp).count() > 1e9;

    auto V = global.adcEval.GetVoltage(adcValue);

    if (adcValuePos == ADC_VALUES_SIZE)
        adcValuePos = 0;
    adcValues[adcValuePos] = V;

    int i = adcValuePos;
    Reset();
    for (int j = 0; j < adcValueCount; ++j, --i)
    {
        auto v = adcValues[i];

        Vmin = min(Vmin, v);
        Vmax = max(Vmax, v);
        Vppk = Vmax - Vmin;

        if (i == 0)
            i = ADC_VALUES_SIZE - 1;
    }

    VminThresHold = Vmin + Vppk * .1;
    VmaxThresHold = Vmin + Vppk * .9;

    if (!freqIsTop && V >= VmaxThresHold)
    {
        freqIsTop = true;
        ++freqCnt;
    }
    else if (freqIsTop && V <= VminThresHold)
    {
        freqIsTop = false;
    }

    if (computeFreq)
    {        
        freq = freqCnt;
        freqCnt = 0;
        refTimestamp = now;
    }

    ++adcValuePos;
    if (adcValueCount < ADC_VALUES_SIZE)
        ++adcValueCount;
}

double SignalStat::GetVmin() { return Vmin; }
double SignalStat::GetVmax() { return Vmax; }
double SignalStat::GetVppk() { return Vppk; }
double SignalStat::GetFreq() { return freq; }
double SignalStat::GetVminThresHold() { return VminThresHold; }
double SignalStat::GetVmaxThresHold() { return VmaxThresHold; }
