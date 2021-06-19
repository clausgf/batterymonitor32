Battery Monitor for ESP32
=========================

This library provides a battery monitor based on the built in ADC of the ESP32 Microcontroller. It is intended for systems without a dedicated power management IC (which is way more accurate).

The battery monitor measures the battery voltage in mV. It allows configuration of a linear calibration curve with slope and offset. The calibration curve also allows to take into account a [voltage divider](https://en.wikipedia.org/wiki/Voltage_divider).

In addition, the battery monitor provides a rough estimate of the remaining battery capacity in percent, which is calculated by linear interpolation between a maximum and a minimum voltage. Although more accurate formulas are availabe, this simple solution is sufficient for many use cases.

Voltage Divider
---------------

The nominal voltage of a LiPo battery used to power an ESP32 is 3.6 V. This library sets the input attenuation to 11 dB, resulting in a voltage range from 0 V to 2.6 V. Thus on the hardware side, I use a voltage divider made from two identical resistors to divide the battery voltage by two. When choosing the resistors, you have to balance between

- current consumption of the two resistors, *I = U / (R_1+R_2)*, e.g. *I = (3.6 V) / (2 * 47 kOhm) = 38* µA and
- additional inaccuracy due to the input impedance of the ADC (which I didn't find in the [ESP32 datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)...).

The ESP32's ADCs is known to be quite inaccuracy, but this improved when Espressif started shipping factory calibrated units. For older devices or improved accuracy, you could use your own a two point calibration. Apply known voltages (e.g. 4.2 V and 3.0 V) to determine the slope and offset for the calibration curve.

Usage
-----

To include and configure the battery monitor use this code:
```cpp
#include "battery_monitor.h"

auto batteryMonitor = BatteryMonitor(
    /*pin*/ 34,                     /* IO34 = ADC Channel 6 */
    /*voltageFactor_perMille*/ 2000,
    /*int voltageBias_mV*/ 150
);
```
The voltage measured on ADC Channel 6 (pin 34) is then corrected by V_adc * 2000  / 1000 + 0.15 V. The constructor also allows to set the 0% and 100% capacity voltages of the battery and to configure the number of voltage samples to average.

A battery readout then looks like this:
```cpp
batteryMonitor.measure();
int batteryVoltage_mV = batteryMonitor.getVoltage_mV();
int batteryPercentage = batteryMonitor.getPercentage();
```
