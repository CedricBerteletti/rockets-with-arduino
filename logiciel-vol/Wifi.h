/*
* Module gérant les connexions wifi
*/

#ifndef Wifi_h
#define Wifi_h

#include "Arduino.h"
#include <WiFiNINA.h>
#include "aides.h"

class Wifi
{
  public:
    Wifi();
    bool actif = false;
    bool loggingIo = false;
    void listerReseaux();
    void init(char ssid[], char pwd[]);
    void logStatut();
    String lireUdp();
    void ecrireUdp(String msg);
  private:
    static const char MODULE_WIFI[];
    static const unsigned int LOCAL_PORT;
    char bufferReception[STRING_MAX_LENGTH];
    char bufferEnvoi[STRING_MAX_LENGTH];
    WiFiUDP udp;
    IPAddress remoteIp;
    unsigned int remotePort;
    String typeChiffrement(int type);
    String ipAddress(IPAddress ip);
    void log(String module, String message, String details, bool toWifi);
};

#endif
