/*
* IMU
*/
#include <Arduino_LSM6DS3.h>  // Bibliothèque pour la gestion de l'IMU (Inertial Measurement Unit)

static const String MODULE_IMU = "IMU";
static const String SUB_MODULE_IMU_DATA = "IMU_DATA";
static const String MODULE_IMU_HEADER = "t, accX, accY, accZ, alphaX, alphaY, alphaZ";

bool imuActif = true;
bool imuLog = true;
int imuDecimales = 2;

void initImu() {
  if (!IMU.begin()) {
    log(MODULE_IMU, "Erreur d'initialisation", "");
    while (true); // Blocage du programme
  }
  
  log(MODULE_IMU, "IMU initialisée", "");
  log(MODULE_IMU, "Fréquence d'échantillonnage des accéléromètres (Hz)", String(IMU.accelerationSampleRate(), 2));
  log(MODULE_IMU, "Fréquence d'échantillonnage des gyroscopes (Hz)", String(IMU.gyroscopeSampleRate(), 2));
}

void lireImu() {
  const String SEPARATEUR = ", ";
 
  if (imuActif
    && IMU.accelerationAvailable()
    && IMU.gyroscopeAvailable()) {
    time = millis();
    IMU.readAcceleration(accX, accY, accZ);
    IMU.readGyroscope(alphaX, alphaY, alphaZ);

    if (imuLog) {
      String data = String(String(time) + SEPARATEUR
        + String(accX, imuDecimales) + SEPARATEUR
        + String(accY, imuDecimales) + SEPARATEUR
        + String(accZ, imuDecimales) + SEPARATEUR
        + String(alphaX, imuDecimales) + SEPARATEUR
        + String(alphaY, imuDecimales) + SEPARATEUR
        + String(alphaZ, imuDecimales));
      log(SUB_MODULE_IMU_DATA, MODULE_IMU_HEADER, data);
    }
  }
}
