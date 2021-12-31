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
    logger.log(MODULE_IMU, "ERROR_INIT", "Erreur d'initialisation");
    while (true); // Blocage du programme
  }
  
  logger.log(MODULE_IMU, "INITIALIZED", "Centrale inertielle initialisée");
  logger.log(MODULE_IMU, "ACCELEROMETER_FREQUENCY", String(IMU.accelerationSampleRate(), 2));
  logger.log(MODULE_IMU, "GYRO_FREQUENCY", String(IMU.gyroscopeSampleRate(), 2));
}

void CentraleInertielle::lire(DonneesInertielles &data) {
  const String SEPARATEUR = ", ";
 
  if (actif
    && IMU.accelerationAvailable()
    && IMU.gyroscopeAvailable()) {
    data.t = millis();
    IMU.readAcceleration(data.accX, data.accY, data.accZ);
    IMU.readGyroscope(data.alphaX, data.alphaY, data.alphaZ);

    if (loggingData) {
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
