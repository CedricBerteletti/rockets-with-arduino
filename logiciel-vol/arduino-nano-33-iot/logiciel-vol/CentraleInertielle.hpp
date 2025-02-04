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
#include "constants.h"
#include "Logger.hpp"
#include "Servomoteur.hpp"


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
    int samplesByDataLog = 0;
    bool rcsActif = false;  // Stabilisation par les ailerons
    bool wcsActif = false;  // Stabilisation par la tuyère orientable
    DonneesInertielles donneesInertiellesFrenetCourantes;
    DonneesInertielles donneesInertiellesTerrestreFrenetCourantes;
    void init();
    void lire();
    void stabiliser();
    void setFiltreMinAcceleration(float minAcc);
    void setFiltreMinVitesseAngulaire(float minV);
    void logDonneesCalibration();
    void calibrer(float ax, float ay, float az, float valpha, float vbeta, float vgamma);
    void setRcsServoX(Servomoteur *servo);
    void setRcsServoY(Servomoteur *servo);
    void setWcsServoX(Servomoteur *servo);
    void setWcsServoY(Servomoteur *servo);
    void setCorrectionFunctionParameters(float a2, float b2, float c2, float d2);
  
  private:
    static const char MODULE_IMU[];
    static const char SOUS_MODULE_IMU_DATA[];
    static const char SOUS_MODULE_IMU_BUFFER[];
    static const char SOUS_MODULE_IMU_STAB[];
    static const char ENTETE_DATA[];
    static const char SEPARATEUR_DATA[];    
    
    // Une seule allocation des buffers/strings au chargement de l'instance
    char strLog[LONGUEUR_MAX_CHAINE_CARACTERES];
    char strNumber[10];
    int index = 0;
    
    // Buffer de données inertielles sur une certaine période (notamment nécessaire pour la calibration)
    DonneesInertielles donneesInertiellesFrenet[TAILLE_BUFFER_DONNEES_INERTIELLES];
    DonneesInertielles donneesInertiellesTerrestre[TAILLE_BUFFER_DONNEES_INERTIELLES];
    
    DonneesInertielles donneesInertiellesFrenetPrecedentes;
    DonneesInertielles donneesInertiellesTerrestrePrecedentes;
    
    // Paramètres de calibration
    float offsetAccX = 0.0f; // Unité : g
    float offsetAccY = 0.0f; // Unité : g
    float offsetAccZ = 0.0f; // Unité : g
    float offsetVAlpha = 0.0f; // Unité : °/s
    float offsetVBeta = 0.0f; // Unité : °/s
    float offsetVGamma = 0.0f; // Unité : °/s
    float minAccX = 0.0f; // Unité : g
    float minAccY = 0.0f; // Unité : g
    float minAccZ = 0.0f; // Unité : g
    float minVAlpha = 0.0f; // Unité : °/s
    float minVBeta = 0.0f; // Unité : °/s
    float minVGamma = 0.0f; // Unité : °/s

    // Paramètre de la fonction de correction (asservissement de la trajectoire), de la forme a.err³ + b.err² + c.err + d
    float a = 0.0f;    
    float b = 0.0f;
    float c = 0.0f;
    float d = 0.0f;

    // Servo-moteurs des systèmes de contrôle
    Servomoteur *rcsServoX;
    Servomoteur *rcsServoY;
    Servomoteur *wcsServoX;
    Servomoteur *wcsServoY;
    
    Logger &logger;
    void log(DonneesInertielles &data, const char module[], const char message[]);
    void log(float xCorrection, float yCorrection, const char module[], const char message[]);

    void integration();
    void stabiliserParAilerons();
    void stabiliserParTuyere();
    float funcCorrection(float var);
    bool isSampleForLog();
};

#endif
