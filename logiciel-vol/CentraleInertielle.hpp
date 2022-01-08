/*
* Gestion de la centrale inertielle de l'Arduino Nano 33 IoT
*/

#ifndef CentraleInertielle_h
#define CentraleInertielle_h

#include <Arduino.h>
#include <Arduino_LSM6DS3.h>  // Bibliothèque pour la gestion de l'IMU (Inertial Measurement Unit)
#include "Logger.hpp"


struct DonneesInertielles {
  unsigned long t = 0;
  float accX = 0.0f;
  float accY = 0.0f;
  float accZ = 0.0f;
  float alphaX = 0.0f;
  float alphaY = 0.0f;
  float alphaZ = 0.0f;
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
    Logger &logger;
};

#endif
