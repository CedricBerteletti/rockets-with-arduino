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
    void lireUdp();
    void ecrireUdp(String msg);
  private:
    static const String MODULE_WIFI;
    static const String MODULE_WIFI_UDP_REC;
    static const String MODULE_WIFI_UDP_SEND;
    static const unsigned int LOCAL_PORT;
    Logger &logger;
    char bufferReception[256];
    char bufferEnvoi[256];
    WiFiUDP udp;
    IPAddress remoteIp;
    unsigned int remotePort;
    String typeChiffrement(int type);
    String ipAddress(IPAddress ip);
};

#endif
