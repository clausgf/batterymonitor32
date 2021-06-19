ConfigManager for ESP32
=======================

Usage
-----

To include and configure the battery monitor use this code:
```cpp
#include "configmanager.h"

const char *defaultConfigStr = "{ status_url: \"http://my.statusurl.org/\" }";
ConfigurationManager defaultConfig = ConfigurationManager("config", defaultConfigStr, 2048);
```
