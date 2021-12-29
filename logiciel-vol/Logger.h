/*
* Logger
*/

#ifndef Logger_h
#define Logger_h

#include "Arduino.h"
#include "aides.h"
#include "Wifi.h"

class Logger
{
  public:
    Logger();
    void log(String module, String message, String details);
    bool toSerial = true;
    bool toSdcard = false;
    bool toUdp = false;
    Wifi *wifi;
};

#endif
