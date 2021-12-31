/*
 * Module principal du programme de vol d'une fusée
 * embarquant un Arduino Nano 33 IoT
 */

#include "CentraleInertielle.h"
#include "Wifi.h"
#include "secrets.h"

static const String MODULE_SYSTEME = "SYSTEM";
static const String MODULE_COMMANDE = "COMMAND";

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
  logger.log(MODULE_SYSTEME, "SYSTEM_INIT", "Initialisation générale");
  initSerial();
  
  wifi.listerReseaux();
  wifi.init(SECRET_SSID, SECRET_PASS);
  wifi.logStatut();

  logger.wifi = &wifi;
  logger.toUdp = true;

  centrale.loggingData = false;
  centrale.init();

  delay(2000);
}

void loop() {
  lireCommande();
  centrale.lire(donneesInertiellesCourantes);
}


void initSerial() {
  Serial.begin(115200);  
  delay(3000);
  logger.log(MODULE_SYSTEME, "SERIAL_INIT", "Liaison série initialisée");
}

void lireCommande() {
  String commande = wifi.lireUdp();
  if(commande.length() > 0) {
    logger.log(MODULE_COMMANDE, "COMMAND_RECEPTION", commande);
    if(commande.startsWith("connect ")) {
      // Ne rien faire : on a déjà récupéré l'IP de l'émetteur dans le module Wifi
    }
    else if (commande.startsWith("toggleLogImuData")) {
      centrale.loggingData = !centrale.loggingData;
    }
  }
}


