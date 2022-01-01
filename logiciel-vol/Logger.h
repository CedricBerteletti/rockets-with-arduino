/*
* Logger
*/

#ifndef Logger_h
#define Logger_h

#include "Arduino.h"
#include "aides.h"
#include "Wifi.h"
#include <SD.h>

class Logger
{
  public:
    Logger();
    void initSdcard();
    void log(String module, String message, String details);
    void flush();
    bool toSerial = true;
    bool toSdcard = false;
    bool toUdp = false;
    Wifi *wifi;
  private:
    static const String MODULE_LOGGER;
    static const String NOM_FICHIER_LOGS;
    Sd2Card carteSd;
    SdVolume volume;
    SdFile root;
    File fichier;
};

#endif
