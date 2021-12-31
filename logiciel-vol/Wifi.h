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
    static const String MODULE_WIFI;
    static const String MODULE_WIFI_UDP_REC;
    static const String MODULE_WIFI_UDP_SEND;
    static const unsigned int LOCAL_PORT;
    char bufferReception[256];
    char bufferEnvoi[256];
    WiFiUDP udp;
    IPAddress remoteIp;
    unsigned int remotePort;
    String typeChiffrement(int type);
    String ipAddress(IPAddress ip);
    void log(String module, String message, String details);
};

#endif
