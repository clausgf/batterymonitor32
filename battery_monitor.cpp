/**
 * Battery Monitor for ESP32
 * Copyright (c) 2021 clausgf@github. See LICENSE.md for legal information.
 */

#include <esp32-hal-adc.h>

#include "battery_monitor.h"


// ***************************************************************************

BatteryMonitor::BatteryMonitor(int pin,
    int voltageFactor_perMille, int voltageBias_mV, 
    int noOfMeasurements, 
    int minVoltage_mV, int maxVoltage_mV)
{
    _pin = pin;
    _voltageFactor_perMille = voltageFactor_perMille;
    _voltageBias_mV = voltageBias_mV;
    _noOfMeasurements = noOfMeasurements;
    _minVoltage_mV = minVoltage_mV;
    _maxVoltage_mV = maxVoltage_mV;
    _voltage_mV = -1;
};

BatteryMonitor& BatteryMonitor::measure()
{
    analogSetWidth(_bits);  // 11 bit is alldgedly more linear than 12 bit
    analogSetAttenuation(ADC_11db);

    uint64_t sum = 0;
    for (int i=0; i<_noOfMeasurements; i++)
    {
        sum += analogReadMilliVolts(_pin);  // requires a newer IDF/Arduino (tested with espressif32@3.2)
    }

    const int adc_mV = sum / _noOfMeasurements;
    _voltage_mV = (adc_mV * _voltageFactor_perMille) / 1000 + _voltageBias_mV;

    return *this;
}

int BatteryMonitor::getVoltage_mV()
{
    if (_voltage_mV < 0)
    {
        measure();
    }
    return _voltage_mV;
}

int BatteryMonitor::getPercentage()
{
    int percentage = -1;
    int voltage_mV = getVoltage_mV();
    if (voltage_mV > _maxVoltage_mV)
    {
        percentage = 100;
    }
    else if (voltage_mV < _minVoltage_mV)
    {
        percentage = 0;
    }
    else
    {
        percentage = (voltage_mV - _minVoltage_mV) * 100 / (_maxVoltage_mV - _minVoltage_mV);
    }
    return percentage;
}
