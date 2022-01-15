/*
 * --------------------------------------------------------------------------
 * Logger
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#ifndef Logger_h
#define Logger_h

#include <Arduino.h>
#include <SD.h>
#include "aides.hpp"
#include "Wifi.hpp"

class Logger
{
  public:
    Logger();
    void initSdcard();
    void logStatut();
    void log(const char module[], const char message[], String details);
    void log(const char module[], const char message[], const char details[]);
    void flush();
    bool toSerial = true;
    bool toSdcard = false;
    bool toUdp = false;
    Wifi *wifi;
  private:
    static const char MODULE_LOGGER[];
    static const char NOM_FICHIER_LOGS[];
    // Une seule allocation des buffers/strings au chargement de l'instance
    char strLog[LONGUEUR_MAX_CHAINE_CARACTERES];
    char strTmp[LONGUEUR_MAX_CHAINE_CARACTERES];
    Sd2Card carteSd;
    SdVolume volume;
    SdFile root;
    File fichier;
};

#endif
