/*
* Module gérant les connexions wifi
*/

#ifndef Wifi_h
#define Wifi_h

#include "Arduino.h"
#include <WiFiNINA.h>
#include "Logger.h"

class Wifi
{
  public:
    Wifi(Logger &logger);
    bool actif = false;
    void listerReseaux();
    void init(char ssid[], char pwd[]);
    void logStatut();
  private:
    static const String MODULE_WIFI;
    Logger &logger;
    String typeChiffrement(int type);
};

#endif
