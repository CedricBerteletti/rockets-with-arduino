/* 
 * --------------------------------------------------------------------------
 * Module principal du programme de vol d'une fusée
 * embarquant un Arduino Nano 33 IoT
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#include "aides.hpp"
#include "Logger.hpp"
#include "Wifi.hpp"
#include "CentraleInertielle.hpp"
#include <Servo.h>

// Nombre d'étapes du vol
#define NB_ETAPES 10

static const char MODULE_SYSTEME[] = "SYSTEM";
static const char MODULE_COMMANDE[] = "COMMAND";
static const long DELAI_FLUSH = 1000; // Flush des logs sur la carte SD toutes les secondes

/* Variable globales */
// Instanciation d'un loggeur
Logger logger;
bool loggingRocketStatus = false;
// Instanciation d'un objet gérant la connexion et les communications wifi
Wifi wifi;
// Instanciation de l'objet gérant la centrale inertielle (constructeur nécessitant un logger en paramètre)
CentraleInertielle centrale(logger);
// Date à laquelle réaliser un nouveau flush des données de logs
// pas à chaque écriture car prendre trop de temps
long flushSuivant = 0;
// Dernière commande reçue (déclaration globale pour éviter la réallocation dans la pile à chaque appel de lireCommande)
char commande[LONGUEUR_MAX_CHAINE_CARACTERES];
// Chaîne de caractère pour les logs
char strLog[LONGUEUR_MAX_CHAINE_CARACTERES];
// Servomoteurs des ailerons
Servo servo[2];

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
int dureeEtape[NB_ETAPES];
char commandeEtape[NB_ETAPES][LONGUEUR_MAX_CHAINE_CARACTERES];
long dateLancement = 0;
long dateEtapeSuivante = -1;
long dateCourante = 0;
DonneesInertielles donneesInertiellesCourantes;


// Initialisation générale de la fusée
void setup() {
  logger.log(MODULE_SYSTEME, "SYSTEM_INIT", "Initialisation générale");
  initSerial();
  delay(1000);

  logger.initSdcard();
  
  wifi.listerReseaux();
  wifi.init();
  wifi.logStatut();

  logger.wifi = &wifi;
  logger.toUdp = true;

  centrale.loggingData = false;
  centrale.init();

  // Blocage des transitions automatiques
  for (int i = 0; i < NB_ETAPES; i++ ) {
    dureeEtape[i] = -1;
  }

  // 2 servomoteurs sur les broches 20 et 21
  servo[0].attach(20);
  servo[1].attach(21);

  delay(1000);
  flushSuivant = millis();
}


// Boucle principale : lecture des capteurs ou des commandes et actions
void loop() {
  lireCommande();
  centrale.lire(donneesInertiellesCourantes);
  
  dateCourante = millis();

  if(dateCourante > flushSuivant) {
    logger.flush();
    flushSuivant = flushSuivant + DELAI_FLUSH;
    if(loggingRocketStatus) {
      logRocketStatus();
    }
  }
  if(dateEtapeSuivante != -1 && dateCourante > dateEtapeSuivante) {
    etapeSuivante();
  }
}


void initSerial() {
  Serial.begin(115200);
  logger.log(MODULE_SYSTEME, "SERIAL_INIT", "Liaison série initialisée");
}

void lireCommande() {
  wifi.lireUdp(commande);
  trim(commande);
  executerCommande(commande);
}

void executerCommande(const char commande[]) {
  if(strlen(commande) > 0) {
    logger.log(MODULE_COMMANDE, "COMMAND_RECEPTION", commande);
    if(chaineCommencePar(commande, "#")) {
      // Ne rien faire : ligne de commentaire
    }
    else if(chaineCommencePar(commande, "connect ")) {
      // Ne rien faire : on a déjà récupéré l'IP de l'émetteur dans le module Wifi
    }
    else if (chaineCommencePar(commande, "initSdcard")) {
      logger.initSdcard();
    }
    else if (chaineCommencePar(commande, "flushLogs")) {
      logger.flush();
    }
    else if (chaineCommencePar(commande, "rocketStatus")) {
      logRocketStatus();
    }
    else if (chaineCommencePar(commande, "rocketSteps")) {
      for (int i = 0; i < NB_ETAPES; i++ ) {
        sprintf(strLog, "%i | %ims | %s", i, dureeEtape[i], commandeEtape[i]);
        logger.log(MODULE_SYSTEME, "ROCKET_STEP", strLog);
      }
    }
    else if (chaineCommencePar(commande, "wifiStatus")) {
      wifi.logStatut();
    }
    else if (chaineCommencePar(commande, "loggerStatus")) {
      logger.logStatut();
    }
    else if (chaineCommencePar(commande, "toggleLogImuData")) {
      centrale.loggingData = !centrale.loggingData;
    }
    else if (chaineCommencePar(commande, "toggleLogFlush")) {
      logger.loggingFlush = !logger.loggingFlush;
    }
    else if (chaineCommencePar(commande, "toggleLogRocketStatus")) {
      loggingRocketStatus = !loggingRocketStatus;
    }
    else if (chaineCommencePar(commande, "configureStep ")) { 
      char chEtape[LONGUEUR_NOMBRE];
      char chDelai[LONGUEUR_NOMBRE];
      copierToken(commande, " ", 1, chEtape);
      copierToken(commande, " ", 2, chDelai);
      int etape = atoi(chEtape);
      int delai = atoi(chDelai);
      dureeEtape[etape] = delai;
      copierToken(commande, " ", 3, commandeEtape[etape], true);
    }
    else if (chaineCommencePar(commande, "launch ") || chaineCommencePar(commande, "stage ")) { // Vérification du code
      if(chaineContient(commande, SECRET_COMMAND_CODE)) {
        etapeSuivante();
      }
      else {
        logger.log(MODULE_COMMANDE, "COMMAND_ERROR_CODE", "Code de derrouillage de la commande incorrect");
      }
    }
    else if (chaineCommencePar(commande, "stop ")) {
      if(chaineContient(commande, SECRET_COMMAND_CODE)) {
        fuseeStatut = -1;
      }
      else {
        logger.log(MODULE_COMMANDE, "COMMAND_ERROR_CODE", "Code de derrouillage de la commande incorrect");
      }
    }
    else if (chaineCommencePar(commande, "delay ")) {
      char chDelai[LONGUEUR_NOMBRE];
      copierToken(commande, " ", 1, chDelai);
      int delai = atoi(chDelai);
      delay(delai);
    }
    else if (chaineCommencePar(commande, "pinMode ")) {
      char chPin[LONGUEUR_NOMBRE];
      char chMode[LONGUEUR_NOMBRE];
      copierToken(commande, " ", 1, chPin);
      copierToken(commande, " ", 2, chMode);
      int pin = atoi(chPin);
      int mode;
      if(chaineCommencePar(chMode, "OUTPUT")) {
        mode = OUTPUT;
      }
      else {
        mode = INPUT;
      }
      pinMode(pin, mode);
    }
    else if (chaineCommencePar(commande, "digitalWrite ")) {
      char chPin[LONGUEUR_NOMBRE];
      char chNiveau[LONGUEUR_NOMBRE];
      copierToken(commande, " ", 1, chPin);
      copierToken(commande, " ", 2, chNiveau);
      int pin = atoi(chPin);
      int niveau = atoi(chNiveau);
      digitalWrite(pin, niveau);
    }
    else if (chaineCommencePar(commande, "servoPosition ")) {
      char chServo[LONGUEUR_NOMBRE];
      char chPosition[LONGUEUR_NOMBRE];
      copierToken(commande, " ", 1, chServo);
      copierToken(commande, " ", 2, chPosition);
      int serv = atoi(chServo);
      int pos = atoi(chPosition);
      servo[serv].write(pos);
    }
    else {
      logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
    }
  }
}

void logRocketStatus() {
  if(fuseeStatut > INITIAL) {
    int dateEnSeconde = (dateCourante - dateLancement - dureeEtape[0])/1000;
    sprintf(strLog, "t=%is | Etape %i", dateEnSeconde, fuseeStatut);
  }
  else {
    sprintf(strLog, "En attente | Etape %i", fuseeStatut);
  }
  logger.log(MODULE_SYSTEME, "ROCKET_STATUS", strLog);
}

void etapeSuivante() {
  fuseeStatut = fuseeStatut + 1;
  if(fuseeStatut > INITIAL) {
    if(fuseeStatut == DECOMPTE_FINAL) {
      dateLancement = millis();
      loggingRocketStatus = true;
      dateEtapeSuivante = dateLancement;
    }

    itoa(fuseeStatut, strLog, 10);
    logger.log(MODULE_SYSTEME, "STAGGING", strLog);

    if(dureeEtape[fuseeStatut] != -1) {
      // La condition de passage à l'étape suivante est une durée
      dateEtapeSuivante = dateEtapeSuivante + dureeEtape[fuseeStatut];
    }
    else {
      // La condition de passage à l'étape suivante n'est pas une durée
      dateEtapeSuivante = -1;
    }
    executerCommande(commandeEtape[fuseeStatut]);
  }  
}


