#include <Arduino_LSM6DS3.h>  // Bibliothèque pour la gestion de l'IMU (Inertial Measurement Unit)

/*
 * Système
 */
static const String MODULE_SYSTEM = "SYSTEM";
unsigned long time = 0;  //Durée en millisecondes depuis le démarrage de l'Arduino

/*
 * IMU
 */
static const String MODULE_IMU = "IMU";
static const String MODULE_IMU_HEADER = "accX, accY, accZ, alphaX, alphaY, alphaZ ";
float accX = 0.0f;
float accY = 0.0f;
float accZ = 0.0f;
float alphaX = 0.0f;
float alphaY = 0.0f;
float alphaZ = 0.0f;


void setup() {
  log(MODULE_SYSTEM, "Initialisation générale", "");
  
  Serial.begin(250000);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  log(MODULE_SYSTEM, "Liaison série initialisée", "");
  
  if (!IMU.begin()) {
    log(MODULE_IMU, "Erreur d'initialisation", "");
    while (true); // Blocage du programme
  }
  
  log(MODULE_IMU, "IMU initialisée", "");
  log(MODULE_IMU, "Fréquence d'échantillonnage des accéléromètres (Hz)", String(IMU.accelerationSampleRate(), 2));
  log(MODULE_IMU, "Fréquence d'échantillonnage des gyroscopes (Hz)", String(IMU.gyroscopeSampleRate(), 2));

  delay(2000);
}

void loop() {
  lireImu();
}

void lireImu() {
  const String SEPARATEUR = ", ";
  const int DECIMALES = 3;
 
  if (IMU.accelerationAvailable()
    && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(accX, accY, accZ);
    IMU.readGyroscope(alphaX, alphaY, alphaZ);

    String data = String(String(accX, DECIMALES) + SEPARATEUR
      + String(accY, DECIMALES) + SEPARATEUR
      + String(accZ, DECIMALES) + SEPARATEUR
      + String(alphaX, DECIMALES) + SEPARATEUR
      + String(alphaY, DECIMALES) + SEPARATEUR
      + String(alphaZ, DECIMALES));

    log(MODULE_IMU, MODULE_IMU_HEADER, data);
  }
}

// Fonction d'écriture des logs (vers le port série)
void log(String module, String message, String details) {
  const String SEPARATEUR = ", ";
  const String GUILLEMETS = "\"";

  time = millis();

  String log = String(String(time) + SEPARATEUR
    + module + SEPARATEUR
    + GUILLEMETS + message + GUILLEMETS + SEPARATEUR
    + GUILLEMETS + details + GUILLEMETS);

  Serial.println(log);  
}
