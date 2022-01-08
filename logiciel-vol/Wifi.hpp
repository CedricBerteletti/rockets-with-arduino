/*
* Module gérant les connexions wifi
*/

#ifndef Wifi_h
#define Wifi_h


#include <WiFiNINA.h>
#include "aides.hpp"
#include "secrets.h"

class Wifi
{
  public:
    Wifi();
    bool actif = false;
    bool loggingIo = false;
    void listerReseaux();
    void init();
    void logStatut();
    void lireUdp(char message[]);
    void ecrireUdp(const char message[]);
  private:
    static const char MODULE_WIFI[];
    static const unsigned int PORT_LOCAL;
    // Une seule allocation des strings au chargement de l'instance
    char strLog[LONGUEUR_MAX_CHAINE_CARACTERES];
    char strTmp[LONGUEUR_MAX_CHAINE_CARACTERES];
    WiFiUDP udp;
    IPAddress remoteIp;
    unsigned int remotePort;
    void init(const char ssid[], const char pwd[]);
    void typeChiffrement(char str[], int type);
    void adresseIp(char addresse[], IPAddress ip);
    void log(const char module[], const char message[], const char details[], bool toWifi);
    void logIo(const char parameter[], const char message[]);
};

#endif
