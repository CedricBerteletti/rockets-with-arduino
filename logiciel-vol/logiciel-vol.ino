/*
 * Module principal du programme de vol d'une fusée
 * embarquant un Arduino Nano 33 IoT
 */

#include "CentraleInertielle.h"
#include "Wifi.h"
#include "secrets.h"

static const String MODULE_SYSTEME = "SYSTEM";

/* Variable globales */
// Instanciation d'un loggeur
Logger logger;
// Instanciation d'un objet gérant la connexion et les communications wifi
Wifi wifi;
// Instanciation d'un objet pour stocker les valeurs courantes des accéléromètres et gyroscopes
DonneesInertielles donneesInertiellesCourantes;
// Instanciation de l'objet gérant la centrale inertielle (constructeur nécessitant un logger en paramètre)
CentraleInertielle centrale(logger);

void setup() {
  logger.log(MODULE_SYSTEME, "Initialisation générale", "");
  initSerial();
  
  wifi.listerReseaux();
  wifi.init(SECRET_SSID, SECRET_PASS);
  wifi.logStatut();
  logger.wifi = &wifi;
  logger.toUdp = true;

  centrale.loggingData = false;
  centrale.init();

  delay(2000);
  centrale.loggingData = true;
}

void loop() {
  wifi.lireUdp();

  centrale.lire(donneesInertiellesCourantes);
}


void initSerial() {
  Serial.begin(115200);  
  delay(3000);
  logger.log(MODULE_SYSTEME, "Liaison série initialisée", "");
}
