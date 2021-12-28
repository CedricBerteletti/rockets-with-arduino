/*
* Gestion de la centrale inertielle de l'Arduino Nano 33 IoT
*/

#include "CentraleInertielle.h"

const String CentraleInertielle::MODULE_IMU = "IMU";
const String CentraleInertielle::SUB_MODULE_IMU_DATA = "IMU_DATA";
const String CentraleInertielle::MODULE_IMU_HEADER = "t, accX, accY, accZ, alphaX, alphaY, alphaZ";

CentraleInertielle::CentraleInertielle(Logger &logger):logger(logger) {
}

void CentraleInertielle::init()
{
  if (!IMU.begin()) {
    logger.log(MODULE_IMU, "Erreur d'initialisation", "");
    while (true); // Blocage du programme
  }
  
  logger.log(MODULE_IMU, "IMU initialisée", "");
  logger.log(MODULE_IMU, "Fréquence d'échantillonnage des accéléromètres (Hz)", String(IMU.accelerationSampleRate(), 2));
  logger.log(MODULE_IMU, "Fréquence d'échantillonnage des gyroscopes (Hz)", String(IMU.gyroscopeSampleRate(), 2));
}

void CentraleInertielle::lire(DonneesInertielles &data) {
  const String SEPARATEUR = ", ";
 
  if (actif
    && IMU.accelerationAvailable()
    && IMU.gyroscopeAvailable()) {
    data.t = millis();
    IMU.readAcceleration(data.accX, data.accY, data.accZ);
    IMU.readGyroscope(data.alphaX, data.alphaY, data.alphaZ);

    if (logging) {
      String str = String(millis()) + SEPARATEUR
        + String(data.accX, decimales) + SEPARATEUR
        + String(data.accY, decimales) + SEPARATEUR
        + String(data.accZ, decimales) + SEPARATEUR
        + String(data.alphaX, decimales) + SEPARATEUR
        + String(data.alphaY, decimales) + SEPARATEUR
        + String(data.alphaZ, decimales);
      logger.log(SUB_MODULE_IMU_DATA, MODULE_IMU_HEADER, str);
    }
  }
}
