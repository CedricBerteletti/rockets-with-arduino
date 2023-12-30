/*
 * --------------------------------------------------------------------------
 * Gestion de la centrale inertielle de l'Arduino Nano 33 IoT
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#include "CentraleInertielle.hpp"

const char CentraleInertielle::MODULE_IMU[] = "IMU";
const char CentraleInertielle::SOUS_MODULE_IMU_DATA[] = "IMU_DATA";
const char CentraleInertielle::SOUS_MODULE_IMU_BUFFER[] = "IMU_BUFFER";
const char CentraleInertielle::ENTETE_DATA[] = "t, accX, accY, accZ, vAlpha, vBeta, vGamma";
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

  index = 0;
}

void CentraleInertielle::lire() {
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    donneesInertiellesCourantes.t = millis();
    IMU.readAcceleration(donneesInertiellesCourantes.accX, donneesInertiellesCourantes.accY, donneesInertiellesCourantes.accZ);
    IMU.readGyroscope(donneesInertiellesCourantes.vAlpha, donneesInertiellesCourantes.vBeta, donneesInertiellesCourantes.vGamma);

    // TODO : calibrer et filtrer les valeurs
    
    // On sauvegarde les valeurs courantes dans le buffer des données inertielles
    donneesInertielles[index] = donneesInertiellesCourantes;
    // La prochaine fois, on stockera la valeur lue dans la "case" suivante du buffer
    index++;
    if(index == TAILLE_BUFFER_DONNEES_INERTIELLES) {
      index = 0;
    }
    
    if (loggingData) {
      log(donneesInertiellesCourantes, SOUS_MODULE_IMU_DATA, ENTETE_DATA);
    }
  }
}

void CentraleInertielle::setMinAccelerationFilter(float minAcc) {
  minAccX = minAcc;
  minAccY = minAcc;
  minAccZ = minAcc;
}

void CentraleInertielle::setMinAngularVelocityFilter(float minV) {
  minVAlpha = minV;
  minVBeta = minV;
  minVGamma = minV;
}

void CentraleInertielle::calibrate(float ax, float ay, float az, float valpha, float vbeta, float vgamma) {
  float axSum = 0;
  float aySum = 0;
  float azSum = 0;
  float valphaSum = 0;
  float vbetaSum = 0;
  float vgammaSum = 0;

  for(int i = 0; i < TAILLE_BUFFER_DONNEES_INERTIELLES; ++i) {
    axSum += donneesInertielles[i].accX;
    aySum += donneesInertielles[i].accY;
    azSum += donneesInertielles[i].accZ;
    valphaSum += donneesInertielles[i].vAlpha;
    vbetaSum += donneesInertielles[i].vBeta;
    vgammaSum += donneesInertielles[i].vGamma;

  offsetAccX = ax - axSum/TAILLE_BUFFER_DONNEES_INERTIELLES;
  offsetAccY = ay - aySum/TAILLE_BUFFER_DONNEES_INERTIELLES;
  offsetAccZ = az - azSum/TAILLE_BUFFER_DONNEES_INERTIELLES;
  offsetVAlpha = valpha - valphaSum/TAILLE_BUFFER_DONNEES_INERTIELLES; 
  offsetVBeta = vbeta - vbetaSum/TAILLE_BUFFER_DONNEES_INERTIELLES;
  offsetVGamma = vgamma - vgammaSum/TAILLE_BUFFER_DONNEES_INERTIELLES;
  }
}

void CentraleInertielle::logBuffer() {
  logger.log(SOUS_MODULE_IMU_BUFFER, "BUFFER_BEGIN", "Début des données dans le cache de calibration");
  delay(DELAI_ENVOI_UDP);
  for (unsigned int i = 0; i < TAILLE_BUFFER_DONNEES_INERTIELLES; i++)  
  {
    log(donneesInertielles[i], SOUS_MODULE_IMU_BUFFER, ENTETE_DATA);
    delay(DELAI_ENVOI_UDP);
  }
  delay(DELAI_ENVOI_UDP);
  logger.log(SOUS_MODULE_IMU_BUFFER, "BUFFER_END", "Fin des données dans le cache de calibration");
}

void CentraleInertielle::log(DonneesInertielles &data, const char module[], const char message[]) {
  Serial.println("Log IMU");
  int longueur = 5 + decimales;
  itoa(data.t, strLog, 10);
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
  
  dtostrf(data.vAlpha, longueur, decimales, strNumber);
  strcat(strLog, strNumber);
  strcat(strLog, SEPARATEUR_DATA);
  
  dtostrf(data.vBeta, longueur, decimales, strNumber);
  strcat(strLog, strNumber);
  strcat(strLog, SEPARATEUR_DATA);
  
  dtostrf(data.vGamma, longueur, decimales, strNumber);
  strcat(strLog, strNumber);

  logger.log(module, message, strLog);
}