/**
 * Battery Monitor for ESP32
 * Copyright (c) 2021 clausgf@github. See LICENSE.md for legal information.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include <esp32-hal-adc.h>

#include <battery_monitor.h>

#include "secrets.h"


static const int ADC_PIN = 34;
static const int NUM_AVGS = 10;

static auto battery = BatteryMonitor(/*pin*/ADC_PIN,
        /*voltageFactor_perMille*/2000, /*voltageBias_mV*/0, 
        /*noOfMeasurements*/10, 
        /*minVoltage_mV*/3400, /*maxVoltage_mV*/4200);


static const char* udpAddress = "192.168.178.20";
static const int udpPort = 10000;

static WiFiUDP udp;

// ***************************************************************************
//             SETUP
// ***************************************************************************

void setup()
{
    delay(500);  // wait for serial interface to get up

    Serial.begin(115200);
    Serial.println("----------------------------------------------");
    Serial.println("Startup");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("Connected to %s\n", WIFI_SSID);
    Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());

    udp.begin(udpPort);

    Serial.println("----------------------------------------------");
    Serial.println("Finished startup");
    Serial.println("----------------------------------------------");
}


// ***************************************************************************
//              LOOP
// ***************************************************************************

static int counter = 0;

void loop()
{
    // measure voltage via ADC
    analogSetWidth(11);  // 11 bit is alldgedly more linear than 12 bit
    analogSetAttenuation(ADC_11db);

    uint32_t _raw = 0;
    for (int i = 0; i < NUM_AVGS; i++)
        _raw += analogRead(ADC_PIN);
    uint16_t raw = _raw / NUM_AVGS;

    uint64_t _milliVolts = 0;
    for (int i = 0; i < NUM_AVGS; i++)
        _milliVolts += analogReadMilliVolts(ADC_PIN);
    uint32_t milliVolts = _milliVolts / NUM_AVGS;

    // measure voltage via BatteryMonitor
    battery.measure();
    int batteryVoltage_mV = battery.getVoltage_mV();
    int batteryPercentage = battery.getPercentage();

    // output the results
    char buf[1024];
    int n = snprintf(buf, 1023, "#%4d | @%8ld | %5d counts | %8u V | %4d mV | %d%%\n", 
        counter, millis(), raw, milliVolts, batteryVoltage_mV, batteryPercentage);
    Serial.print(buf);

    udp.beginPacket(udpAddress, udpPort);
    udp.write((uint8_t *)buf, n);
    udp.endPacket();

    // end the loop
    counter++;
    delay(1000);
}
