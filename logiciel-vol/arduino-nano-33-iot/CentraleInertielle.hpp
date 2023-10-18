/*
 * --------------------------------------------------------------------------
 * Gestion de la centrale inertielle de l'Arduino Nano 33 IoT
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#ifndef CentraleInertielle_h
#define CentraleInertielle_h

#include <Arduino.h>
#include <Arduino_LSM6DS3.h>  // Bibliothèque pour la gestion de l'IMU (Inertial Measurement Unit)
#include <avr/dtostrf.h>
#include "Logger.hpp"


struct DonneesInertielles {
  unsigned long t = 0;
  float accX = 0.0f; // Unité : g
  float accY = 0.0f; // Unité : g
  float accZ = 0.0f; // Unité : g
  float alphaX = 0.0f; // Unité : °/s
  float alphaY = 0.0f; // Unité : °/s
  float alphaZ = 0.0f; // Unité : °/s
};

class CentraleInertielle
{
  public:
    CentraleInertielle(Logger &logger);
    bool actif = true;
    bool loggingData = true;
    int decimales = 2;
    void init();
    void lire(DonneesInertielles &data);
  private:
    static const char MODULE_IMU[];
    static const char SOUS_MODULE_IMU_DATA[];
    static const char ENTETE_DATA[];
    static const char SEPARATEUR_DATA[];
    // Une seule allocation des buffers/strings au chargement de l'instance
    char strLog[LONGUEUR_MAX_CHAINE_CARACTERES];
    char strNumber[10];
    Logger &logger;
};

#endif
