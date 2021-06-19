Battery Monitor Example
=======================

- Measure an input voltage with the ADC directly and using BatteryMonitor
- Publish the results via Serial and UDP (configure receiving computer in the `udpAddress` variable)
- Create a secrets.h with your WiFi credentials
- Display the results with
  ```sh
  nc -ul 10000
  ```
