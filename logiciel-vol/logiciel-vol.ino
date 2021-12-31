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
// Instanciation de l'objet gérant la centrale inertielle (constructeur nécessitant un logger en paramètre)
CentraleInertielle centrale(logger);

/* Etat de la fusée */
static const int INITIAL = -1;
static const int DECOMPTE_FINAL = 0;
static const int ETAGE_1 = 1; // Booster
static const int ETAGE_2 = 2; // Etage supérieur
static const int ETAGE_3 = 3; // Capsule
static const int SUITE_1 = 4;
static const int SUITE_2 = 5;
static const int SUITE_3 = 6;
static const int SUITE_4 = 7;
static const int SUITE_5 = 8;
static const int SUITE_6 = 9;
int fuseeStatut = INITIAL;

/* Données de navigation */
// Instanciation d'un objet pour stocker les valeurs courantes des accéléromètres et gyroscopes
DonneesInertielles donneesInertiellesCourantes;
long dateLancement = 0;

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
  executerCommande(commande);
}

void executerCommande(String commande) {
  if(commande.length() > 0) {
    logger.log(MODULE_COMMANDE, "COMMAND_RECEPTION", commande);
    if(commande.startsWith("connect ")) {
      // Ne rien faire : on a déjà récupéré l'IP de l'émetteur dans le module Wifi
    }
    else if (commande.startsWith("rocketStatus")) {
      logger.log(MODULE_SYSTEME, "ROCKET_STATUS", String(fuseeStatut));
    }
    else if (commande.startsWith("wifiStatus")) {
      wifi.logStatut();
    }
    else if (commande.startsWith("toggleLogImuData")) {
      centrale.loggingData = !centrale.loggingData;
    }
    // Perte de la connexion en cours
    // else if (commande.startsWith("wifiNetworks")) {
    //   wifi.listerReseaux();
    // }
    else if (commande.startsWith("configureStep ")) {
      // TODO
    }
    else if (commande.startsWith("launch ")) {
      etapeSuivante();
    }
    else if (commande.startsWith("stage ")) {
      etapeSuivante();
    }
    else if (commande.startsWith("stop ")) {
      fuseeStatut = -1;
    }
    else if (commande.startsWith("digitalWrite ")) {
      // TODO
    }
  }
}

void etapeSuivante() {
  fuseeStatut = fuseeStatut + 1;
  if(fuseeStatut >= 0) {
    if(fuseeStatut == 0) dateLancement = millis();
    // TODO
  }
}


