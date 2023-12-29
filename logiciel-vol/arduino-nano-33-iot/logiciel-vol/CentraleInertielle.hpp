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
  unsigned long t = 0; // Unité : ms
  float accX = 0.0f; // Unité : g
  float accY = 0.0f; // Unité : g
  float accZ = 0.0f; // Unité : g

  float vX = 0.0f; // Unité : m/s
  float vY = 0.0f; // Unité : m/s
  float vZ = 0.0f; // Unité : m/s

  float x = 0.0f; // Unité : m
  float y = 0.0f; // Unité : m
  float z = 0.0f; // Unité : m
  
  float vAlpha = 0.0f; // Unité : °/s
  float vBeta = 0.0f; // Unité : °/s
  float vGamma = 0.0f; // Unité : °/s

  float alpha = 0.0f; // Unité : °
  float beta = 0.0f; // Unité : °
  float gamma = 0.0f; // Unité : °
};

class CentraleInertielle
{
  public:
    CentraleInertielle(Logger &logger);
    bool loggingData = false;
    int decimales = 2;    
    DonneesInertielles donneesInertiellesCourantes;
    void init();
    void lire();
    void setMinAccelerationFilter(float minAcc);
    void setMinAngularVelocityFilter(float minV);
    void logBuffer();
  
  private:
    static const char MODULE_IMU[];
    static const char SOUS_MODULE_IMU_DATA[];
    static const char SOUS_MODULE_IMU_BUFFER[];
    static const char ENTETE_DATA[];
    static const char SEPARATEUR_DATA[];
    static const int TAILLE_BUFFER_DONNEES_INERTIELLES = 100;
    
    // Une seule allocation des buffers/strings au chargement de l'instance
    char strLog[LONGUEUR_MAX_CHAINE_CARACTERES];
    char strNumber[10];
    int index = 0;
    
    // Buffer de données inertielles sur une certaine période (notamment nécessaire pour la calibration)
    DonneesInertielles donneesInertielles[TAILLE_BUFFER_DONNEES_INERTIELLES];
    
    // Paramètres de calibration
    float offsetAccX = 0.0f; // Unité : g
    float offsetAccY = 0.0f; // Unité : g
    float offsetAccZ = 0.0f; // Unité : g
    float offsetAlpha = 0.0f; // Unité : °/s
    float offsetBeta = 0.0f; // Unité : °/s
    float offsetGamma = 0.0f; // Unité : °/s
    float minAccX = 0.0f; // Unité : g
    float minAccY = 0.0f; // Unité : g
    float minAccZ = 0.0f; // Unité : g
    float minVAlpha = 0.0f; // Unité : °/s
    float minVBeta = 0.0f; // Unité : °/s
    float minVGamma = 0.0f; // Unité : °/s
    
    Logger &logger;
    void log(DonneesInertielles &data, const char module[], const char message[]);
};

#endif
