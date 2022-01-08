/*
* Gestion de la centrale inertielle de l'Arduino Nano 33 IoT
*/

#include "CentraleInertielle.hpp"

const char CentraleInertielle::MODULE_IMU[] = "IMU";
const char CentraleInertielle::SOUS_MODULE_IMU_DATA[] = "IMU_DATA";
const char CentraleInertielle::ENTETE_DATA[] = "t, accX, accY, accZ, alphaX, alphaY, alphaZ";
const char CentraleInertielle::SEPARATEUR_DATA[] = ", ";

CentraleInertielle::CentraleInertielle(Logger &logger):logger(logger) {
}

void CentraleInertielle::init()
{
  if (!IMU.begin()) {
    logger.log(MODULE_IMU, "ERROR_INIT", "Erreur d'initialisation");
    while (true); // Blocage du programme
  }
  
  logger.log(MODULE_IMU, "INITIALIZED", "Centrale inertielle initialisée");
  itoa(IMU.accelerationSampleRate(), strLog, 10);
  logger.log(MODULE_IMU, "ACCELEROMETER_FREQUENCY", strLog);
  itoa(IMU.gyroscopeSampleRate(), strLog, 10);
  logger.log(MODULE_IMU, "GYRO_FREQUENCY", strLog);
}

void CentraleInertielle::lire(DonneesInertielles &data) {
  if (actif
    && IMU.accelerationAvailable()
    && IMU.gyroscopeAvailable()) {
    data.t = millis();
    IMU.readAcceleration(data.accX, data.accY, data.accZ);
    IMU.readGyroscope(data.alphaX, data.alphaY, data.alphaZ);

    if (loggingData) {
      int longueur = 5 + decimales;
      itoa(millis(), strLog, 10);
      strcat(strLog, SEPARATEUR_DATA);

      dtostrf(data.accX, longueur, decimales, strNumber);
      strcat(strLog, strNumber);
      strcat(strLog, SEPARATEUR_DATA);
      
      dtostrf(data.accY, longueur, decimales, strNumber);
      strcat(strLog, strNumber);
      strcat(strLog, SEPARATEUR_DATA);
      
      dtostrf(data.accZ, longueur, decimales, strNumber);
      strcat(strLog, strNumber);
      strcat(strLog, SEPARATEUR_DATA);
      
      dtostrf(data.alphaX, longueur, decimales, strNumber);
      strcat(strLog, strNumber);
      strcat(strLog, SEPARATEUR_DATA);
      
      dtostrf(data.alphaY, longueur, decimales, strNumber);
      strcat(strLog, strNumber);
      strcat(strLog, SEPARATEUR_DATA);
      
      dtostrf(data.alphaZ, longueur, decimales, strNumber);
      strcat(strLog, strNumber);

      logger.log(SOUS_MODULE_IMU_DATA, ENTETE_DATA, strLog);
    }
  }
}
