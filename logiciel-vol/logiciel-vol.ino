/*
 * Module principal du programme de vol d'une fusée
 * embarquant un Arduino Nano 33 IoT
 */

#include "Logger.h"
#include "CentraleInertielle.h"
#include "Wifi.h"
#include "secrets.h"

// Nombre d'étapes du vol
#define NB_ETAPES 10

static const String MODULE_SYSTEME = "SYSTEM";
static const String MODULE_COMMANDE = "COMMAND";
static const long DELAI_FLUSH = 1000; // Flush des logs sur la carte SD toutes les secondes

/* Variable globales */
// Instanciation d'un loggeur
Logger logger;
// Instanciation d'un objet gérant la connexion et les communications wifi
Wifi wifi;
// Instanciation de l'objet gérant la centrale inertielle (constructeur nécessitant un logger en paramètre)
CentraleInertielle centrale(logger);
// Date à laquelle réaliser un nouveau flush des données de logs
// pas à chaque écriture car prendre trop de temps
long flushSuivant = 0;

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
int dureeEtape[NB_ETAPES];
String commandeEtape[NB_ETAPES];
long dateEtapeSuivante = -1;

void setup() {
  logger.log(MODULE_SYSTEME, "SYSTEM_INIT", "Initialisation générale");
  initSerial();
  delay(1000);
  logger.initSdcard();
  
  wifi.listerReseaux();
  wifi.init(SECRET_SSID, SECRET_PASS);
  wifi.logStatut();

  logger.wifi = &wifi;
  logger.toUdp = true;

  centrale.loggingData = false;
  centrale.init();

  delay(1000);
  flushSuivant = millis();
}

void loop() {
  lireCommande();
  centrale.lire(donneesInertiellesCourantes);
  long date = millis();
  if(date > flushSuivant) {
    logger.flush();
    flushSuivant = flushSuivant + DELAI_FLUSH;
  }
  if(dateEtapeSuivante != -1 && date > dateEtapeSuivante) {
    etapeSuivante();
  }
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
    else if (commande.startsWith("initSdcard")) {
      logger.initSdcard();
    }
    else if (commande.startsWith("flushLogs")) {
      logger.flush();
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
    // Perte de la connexion en cours de commande
    // else if (commande.startsWith("wifiNetworks")) {
    //   wifi.listerReseaux();
    // }
    else if (commande.startsWith("configureStep ")) {
      // TODO
    }
    else if (commande.startsWith("launch ")) {
      // TODO code
      etapeSuivante();
    }
    else if (commande.startsWith("stage ")) {
      // TODO code
      etapeSuivante();
    }
    else if (commande.startsWith("stop ")) {
      // TODO code
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
    if(fuseeStatut == 0) {
      dateLancement = millis();
      dateEtapeSuivante = dateLancement;
    }
    logger.log(MODULE_SYSTEME, "STAGGING", String(fuseeStatut));
    dateEtapeSuivante = dateEtapeSuivante + dureeEtape[fuseeStatut];
    executerCommande(commandeEtape[fuseeStatut]);
  }  
}


