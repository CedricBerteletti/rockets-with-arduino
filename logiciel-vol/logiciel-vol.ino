/*
 * Module principal du programme de vol d'une fusée
 * embarquant un Arduino Nano 33 IoT
 */

#include "CentraleInertielle.h"

static const String MODULE_SYSTEME = "SYSTÈME";

/* Variable globales */
// Instanciation d'un loggeur
Logger logger; 
// Instanciation d'un objet pour stocker les valeurs courantes des accéléromètres et gyroscopes
DonneesInertielles donneesInertiellesCourantes;
// Instanciation de l'objet gérant la centrale inertielle (constructeur nécessitant un logger en paramètre)
CentraleInertielle centrale(logger);

void setup() {
  logger.log(MODULE_SYSTEME, "Initialisation générale", "");
  initSerial();
  centrale.init();

  delay(2000);
}

void loop() {
  centrale.lire(donneesInertiellesCourantes);
}


void initSerial() {
  Serial.begin(250000);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
  logger.log(MODULE_SYSTEME, "Liaison série initialisée", "");
}
