#include <Arduino_LSM6DS3.h>  // Bibliothèque pour la gestion de l'IMU (Inertial Measurement Unit)
#include <avr/dtostrf.h>      // Fonction pour la conversion des nombres décimaux en chaînes de caractères

/*
 * Système
 */
static const char MODULE_SYSTEM[] = "SYSTEM";
unsigned long time = 0;  //Durée en millisecondes depuis le démarrage de l'Arduino

/*
 * IMU
 */
static const char MODULE_IMU[] = "IMU";
static const char MODULE_IMU_HEADER[] = "accX, accY, accZ, alphaX, alphaY, alphaZ ";
float accX = 0.0f;
float accY = 0.0f;
float accZ = 0.0f;
float alphaX = 0.0f;
float alphaY = 0.0f;
float alphaZ = 0.0f;


void setup() {
  log((char *)MODULE_SYSTEM, (char *)"Initialisation générale", (char *)"");
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  log((char *)MODULE_SYSTEM, (char *)"Liaison série initialisée", (char *)"");
  
  if (!IMU.begin()) {
    log((char *)MODULE_IMU, (char *)"Erreur d'initialisation", (char *)"");
    //Serial.println("Erreur d'initialisation de l'IMU !");
    while (true); // Blocage du programme
  }
  
  log((char *)MODULE_IMU, (char *)"IMU initialisée", (char *)"");
  char temp[10];
  log((char *)MODULE_IMU, (char *)"Fréquence d'échantillonnage des accéléromètres (Hz)", dtostrf(IMU.accelerationSampleRate(), 4, 2, temp));
  log((char *)MODULE_IMU, (char *)"Fréquence d'échantillonnage des gyroscopes (Hz)", dtostrf(IMU.gyroscopeSampleRate(), 4, 2, temp));

  delay(1000);
}

void loop() {
  lireImu();
}

void lireImu() {
  const char * separateur = ", ";
  char temp[10];
  char str[70];
 
  if (IMU.accelerationAvailable()
    && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(accX, accY, accZ);
    IMU.readGyroscope(alphaX, alphaY, alphaZ);
    
    strcpy(str, dtostrf(accX, 4, 2, temp));
    strcat(str, separateur);
    strcat(str, dtostrf(accY, 4, 2, temp));
    strcat(str, separateur);
    strcat(str, dtostrf(accZ, 4, 2, temp));
    strcat(str, separateur);
    strcat(str, dtostrf(alphaX, 4, 2, temp));
    strcat(str, separateur);
    strcat(str, dtostrf(alphaX, 4, 2, temp));    
    strcat(str, separateur);
    strcat(str, dtostrf(alphaX, 4, 2, temp));

    log((char *)MODULE_IMU, (char *)MODULE_IMU_HEADER, str);
  }
}

// Fonction d'écriture des logs (vers le port série)
void log(char* module, char* message, char* details) {
  char str[256];
  char temp[10];
  const char * separateur = ", ";
  const char * guillemets = "\"";

  time = millis();
  snprintf(temp, 10, "%lu", time);
  strcpy(str, temp);

  strcat(str, separateur);
  strcat(str, module);

  strcat(str, separateur);
  strcat(str, guillemets);
  strcat(str, message);
  strcat(str, guillemets);

  strcat(str, separateur);
  strcat(str, guillemets);
  strcat(str, details);
  strcat(str, guillemets);

  Serial.println(str);  
}
