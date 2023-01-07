/**
 * Battery Monitor for ESP32
 * Copyright (c) 2021 clausgf@github. See LICENSE.md for legal information.
 */

#pragma once


// ***************************************************************************

/**
 * Battery Monitor
 * 
 * This class uses a built-in ADC for battery voltage measurement.
 * It can be configured for a simple linear calibration curve with slope and 
 * offset to consider a voltage divider and for calibration.
 */
class BatteryMonitor
{
public:
    /**
     * BatteryMonitor Constructor
     * 
     * @param pin ADC capable pin used for voltage measurement
     * @param voltageFactor_perMille Factor in 1/1000 to multiply the measured voltage with for calibration
     * @param voltageBias_mV Voltage bias to add to measured voltage for calibration (after applying voltageFactor_perMille)
     * @param noOfMeasurements Number of measurements to improve accuracy through averaging, defaults to 1
     * @param minVoltage_mV Minimum voltage for an empty battery (0 %), defaults to a conservative 3.4 V
     * @param maxVoltage_mV Maximum voltage for a full battery (100 %), defaults to 4.2 V
     */
    BatteryMonitor(int pin,
        int voltageFactor_perMille = 1000, int voltageBias_mV = 0, 
        int noOfMeasurements = 10, 
        int minVoltage_mV = 3400, int maxVoltage_mV = 4200);

    /**
     * Measure the battery voltage.
     * This function always triggers a new measurement, independent of any
     * previous measurements.
     */
    BatteryMonitor& measure();

    /**
     * Get the battery voltage.
     * If the voltage has not been not measured before, this function also 
     * triggers a measurement.
     * 
     * @return Battery voltage in mV
     */
    int getVoltage_mV();

    /**
     * Get the remaining battery capacity in percent, as derived from its voltage,
     * and based on minimum and maximum voltage given in the constructor.
     * This method gives a rough estimate as it does not consider the strong
     * nonlinearities needed for a more accurate determination.
     * If the voltage has not been not measured before, this function also
     * triggers a measurement.
     * 
     * @return Remaining battery capacity in percent
     */
    int getPercentage();

private:
    int _pin;
    const int _bits = 11;  // 11 bit is alldgedly more linear than 12 bit
    int _voltageFactor_perMille;
    int _voltageBias_mV;
    int _noOfMeasurements;
    int _minVoltage_mV;
    int _maxVoltage_mV;
    int _voltage_mV;
};
