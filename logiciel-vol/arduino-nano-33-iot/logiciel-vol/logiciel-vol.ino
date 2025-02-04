/* 
 * --------------------------------------------------------------------------
 * Module principal du programme de vol d'une fusée
 * embarquant un Arduino Nano 33 IoT
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#include "constants.h"
#include "aides.hpp"
#include "Logger.hpp"
#include "Wifi.hpp"
#include "CentraleInertielle.hpp"
#include "Servomoteur.hpp"
#include "Interpreteur.hpp"


/* Modules */

// Instanciation d'un loggeur
Logger logger;

// Instanciation d'un objet gérant la connexion et les communications wifi
Wifi wifi;

// Instanciation de l'objet gérant la centrale inertielle (constructeur nécessitant un logger en paramètre)
CentraleInertielle centrale(logger);

// Servomoteurs des ailerons et de la tuyère sur les broches 18 à 21
Servomoteur servos[NB_SERVOS] = {Servomoteur(logger), Servomoteur(logger), Servomoteur(logger), Servomoteur(logger)};
int servosBroches[NB_SERVOS] = {SERVO0_PIN, SERVO1_PIN, SERVO2_PIN, SERVO3_PIN};

/* Programme de vol */
ProgrammeVol programmeVol(logger);

// Instanciation de l'interpréteur de commandes
Interpreteur interpreteur(logger, wifi, centrale, programmeVol, servos);


/* Variable globales */
// Date à laquelle réaliser un nouveau flush des données de logs
// (pas à chaque écriture car prend trop de temps)
long dateFlushSuivant = 0;
// Date à laquelle mettre à jour le statut de la fusée
long dateStatutSuivant = 0;
// Date à laquelle MAJ les données inertielles
long dateLectureImuSuivante = 0;
// Date à laquelle vérifier la présence de nouvelles commandes
long dateLectureCommandeSuivante = 0;
// Chaîne de caractères pour les logs
char strLog[LONGUEUR_MAX_CHAINE_CARACTERES];


/* Initialisation générale de la fusée */
void setup() {
  logger.log(MODULE_SYSTEME, "SYSTEM_INIT", "Initialisation générale");
  initSerie();
  delay(1000);

  logger.initCarteSd();
  
  wifi.listerReseaux();
  wifi.connecter();
  wifi.logStatut();

  logger.wifi = &wifi;
  logger.logUdpActif = true;

  // Pour éviter de polluer les logs, on désactive les logs des données inertielles avant le lancement
  centrale.samplesByDataLog = 0;
  centrale.init();

  // Initialisation des valeurs par défaut de calibration des servo-moteurs
  for(int i=0; i<NB_SERVOS; i++) {
    servos[i].init(i, servosBroches[i]);
  }

  delay(1000);
  programmeVol.init(millis());
  dateFlushSuivant = programmeVol.dateCourante;
  dateLectureCommandeSuivante = programmeVol.dateCourante;
}


// Boucle principale : lecture des capteurs ou des commandes et actions
void loop() {
  programmeVol.dateCourante = millis();

  // Actions à fréquence réduite (pas effectuées à chaque itération de loop())

  if(programmeVol.dateCourante > dateLectureImuSuivante) {
    centrale.lire();
    centrale.stabiliser();
    dateLectureImuSuivante = programmeVol.dateCourante + DELAI_IMU;
  }

  if(programmeVol.dateCourante > dateLectureCommandeSuivante) {
    interpreteur.lireCommande();
    dateLectureCommandeSuivante = programmeVol.dateCourante + DELAI_COMMANDE;
  }
  
  if(programmeVol.loggingStatutFusee && programmeVol.dateCourante > dateStatutSuivant) {
    programmeVol.logStatutFusee();
    dateStatutSuivant = programmeVol.dateCourante + DELAI_STATUT;
  }
  
  if(programmeVol.dateCourante > dateFlushSuivant) {
    logger.forcerEcritureSurCarteSd();
    dateFlushSuivant = programmeVol.dateCourante + DELAI_FLUSH;
  }

  if(programmeVol.evaluerEtapeSuivante()) {
    interpreteur.verifierEtExecuterCommande(programmeVol.getCommandeCourante());
  }
}

/* Initialisation de la liaison série (pour débogage sur un PC) */
void initSerie() {
  Serial.begin(115200);
  logger.log(MODULE_SYSTEME, "SERIAL_INIT", "Liaison série initialisée");
}

