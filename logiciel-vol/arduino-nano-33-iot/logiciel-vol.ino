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
#define NB_ETAPES 100

static const char MODULE_SYSTEME[] = "SYSTEM";
static const char MODULE_COMMANDE[] = "COMMAND";
static const long DELAI_FLUSH = 1000; // (en ms) Flush des logs sur la carte SD toutes les secondes
static const long DELAI_COMMANDE = 100; // (en ms) Tentative de lecture d'une nouvelle commande toutes les 100 ms

/* Variable globales */
// Instanciation d'un loggeur
Logger logger;
bool loggingRocketStatus = false;
// Instanciation d'un objet gérant la connexion et les communications wifi
Wifi wifi;
// Instanciation de l'objet gérant la centrale inertielle (constructeur nécessitant un logger en paramètre)
CentraleInertielle centrale(logger);
// Date à laquelle réaliser un nouveau flush des données de logs
// (pas à chaque écriture car prend trop de temps)
long dateFlushSuivant = 0;
// Date à laquelle vérifier la présence de nouvelles commandes
long dateLectureCommandeSuivante = 0;
// Dernière commande reçue (déclaration globale pour éviter la réallocation dans la pile à chaque appel de lireCommande)
char commande[LONGUEUR_MAX_COMMANDE];
// Chaîne de caractère pour les logs
char strLog[LONGUEUR_MAX_CHAINE_CARACTERES];
// Servomoteurs des ailerons sur les broches 20 et 21
Servo servos[2];
static const int SERVO0_PIN = 20;
static const int SERVO1_PIN = 21;

/* Etats de la fusée */
static const int CONFIGURATION = -1;
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
int fuseeStatut = CONFIGURATION;

/* Données de navigation */
int dureeEtape[NB_ETAPES];
char commandeEtape[NB_ETAPES][LONGUEUR_MAX_COMMANDE];
long dateLancement = 0;
long dateEtapeSuivante = -1;
long dateCourante = 0;
// Instanciation d'un objet pour stocker les valeurs courantes des accéléromètres et gyroscopes
DonneesInertielles donneesInertiellesCourantes;


/* Initialisation générale de la fusée */
void setup() {
  logger.log(MODULE_SYSTEME, "SYSTEM_INIT", "Initialisation générale");
  initSerial();
  delay(1000);

  logger.initSdcard();
  
  wifi.listerReseaux();
  wifi.connecter();
  wifi.logStatut();

  logger.wifi = &wifi;
  logger.toUdp = true;

  // Pour éviter de polLuer les logs, on désactive les logs des données inertielles avant le lanCement
  centrale.loggingData = false;
  centrale.init();

  // Blocage des transitions automatiques
  for (int i = 0; i < NB_ETAPES; i++ ) {
    dureeEtape[i] = -1;
  }

  // 2 servomoteurs sur les broches 20 et 21
  servos[0].attach(SERVO0_PIN);
  servos[1].attach(SERVO1_PIN);

  delay(1000);
  dateCourante = millis();
  dateFlushSuivant = dateCourante;
  dateLectureCommandeSuivante = dateCourante;
}


// Boucle principale : lecture des capteurs ou des commandes et actions
void loop() {
  dateCourante = millis();
  centrale.lire(donneesInertiellesCourantes);  

  // Actions à fréquence réduite (pas effectuées à chaque itération de loop())
  if(dateCourante > dateFlushSuivant) {
    logger.flush();
    dateFlushSuivant = dateFlushSuivant + DELAI_FLUSH;
    if(loggingRocketStatus) {
      logRocketStatus();
    }
  }
  if(dateCourante > dateLectureCommandeSuivante) {
    dateLectureCommandeSuivante = dateLectureCommandeSuivante + DELAI_COMMANDE;
    lireCommande();
  }

  if(dateEtapeSuivante != -1 && dateCourante > dateEtapeSuivante) {
    // Stagging: il est temps de passer à l'étage supérieur !
    etapeSuivante();
  }
}

/* Initialisation de la liaison série (pour débogage sur un PC) */
void initSerial() {
  Serial.begin(115200);
  logger.log(MODULE_SYSTEME, "SERIAL_INIT", "Liaison série initialisée");
}

/* Log l'état/étape/étage actuel de la fusée */
void logRocketStatus() {
  if(fuseeStatut > CONFIGURATION) {
    int dateEnSeconde = (dateCourante - dateLancement - dureeEtape[0])/1000;
    sprintf(strLog, "t=%is | Etape %i", dateEnSeconde, fuseeStatut);
  }
  else {
    sprintf(strLog, "En attente | Etape %i", fuseeStatut);
  }
  logger.log(MODULE_SYSTEME, "ROCKET_STATUS", strLog);
}

/* Stagging : la fusée passe à une nouvelle séquence du vol (par exemple, du 1° au 2° étage)
  - on incrémente le statut/étage de la fusée (initalement à -1)
  - on exécute la commande correspondant à cette nouvelle étape
  - on indique la date de la prochaine transition d'étage
*/
void etapeSuivante() {
  fuseeStatut = fuseeStatut + 1;
  if(fuseeStatut > CONFIGURATION) {
    if(fuseeStatut == DECOMPTE_FINAL) {
      // Première étape : le décompte final ! La fusée est en auto à partir de maintenant
      // (ie, transition d'état selon ce qui a été précédemment paramétré)
      dateLancement = millis();
      // On commence à afficher les logs sur les 
      loggingRocketStatus = true;
      // Initialisation de la dateEtapeSuivante à la date courante
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
    verifierEtExecuterCommande(commandeEtape[fuseeStatut]);
  }
}


/* GESTION DES COMMANDES ENVOYEES A LA FUSEE */

void lireCommande() {
  wifi.lireUdp(commande);
  trim(commande);
  verifierEtExecuterCommande(commande);
}

void verifierEtExecuterCommande(const char commande[]) {
  if(strlen(commande) > 0) executerCommande(commande);
}

/* Premier switch sur la classe de commandes, pour rationaliser le nombre de comparaisons
et les variables créées dans la pile */
void executerCommande(const char commande[]) {
  logger.log(MODULE_COMMANDE, "COMMAND_RECEPTION", commande);

  if(chaineCommencePar(commande, "status.")) {
    executerCommandeStatus(commande);
  }
  else if(chaineCommencePar(commande, "pin.")) {
    executerCommandePin(commande);
  }
  else if(chaineCommencePar(commande, "rocket.")) {
    executerCommandeRocket(commande);
  }
  else if(chaineCommencePar(commande, "servo.")) {
    executerCommandeServo(commande);
  }
  else if(chaineCommencePar(commande, "logger.")) {
    executerCommandeLogger(commande);
  }
  else if(chaineCommencePar(commande, "wifi.")) {
    executerCommandeWifi(commande);
  }
  else if(chaineCommencePar(commande, "#")) {
    // Ne rien faire : ligne de commentaire
  }
  else {
    executerCommandeAutre(commande);
  }
}

void executerCommandeStatus(const char commande[]) {
  if(chaineCommencePar(commande, "status.rocket")) {
    logRocketStatus();
  }
  else if(chaineCommencePar(commande, "status.logger")) {
    logger.logStatut();
  }
  else if(chaineCommencePar(commande, "status.wifi")) {
    wifi.logStatut();
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void executerCommandePin(const char commande[]) {
  char chPin[LONGUEUR_NOMBRE];
  char chParam[LONGUEUR_NOMBRE];
  int pin;
  int param;
  if(chaineCommencePar(commande, "pin.setMode ")) {
    copierToken(commande, " ", 1, chPin);
    copierToken(commande, " ", 2, chParam);
    pin = atoi(chPin);
    if(chaineCommencePar(chParam, "OUTPUT")) {
      param = OUTPUT;
    }
    else {
      param = INPUT;
    }
    pinMode(pin, param);
  }
  else if(chaineCommencePar(commande, "pin.digitalWrite ")) {
    copierToken(commande, " ", 1, chPin);
    copierToken(commande, " ", 2, chParam);
    pin = atoi(chPin);
    param = atoi(chParam);
    digitalWrite(pin, param);
  }
  else if(chaineCommencePar(commande, "pin.tone ")) {
    copierToken(commande, " ", 1, chPin);
    copierToken(commande, " ", 2, chParam);
    pin = atoi(chPin);
    param = atoi(chParam);
    tone(pin, param);
  }
  else if(chaineCommencePar(commande, "pin.toneStop ")) {
    copierToken(commande, " ", 1, chPin);
    pin = atoi(chPin);
    noTone(pin);
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void executerCommandeRocket(const char commande[]) {
  char chEtape[LONGUEUR_NOMBRE];
  char chDelai[LONGUEUR_NOMBRE];
  int etape;
  int delai;

  if(chaineCommencePar(commande, "rocket.configureStep ")) {
    copierToken(commande, " ", 1, chEtape);
    copierToken(commande, " ", 2, chDelai);
    etape = atoi(chEtape);
    delai = atoi(chDelai);
    dureeEtape[etape] = delai;
    copierToken(commande, " ", 3, commandeEtape[etape], true);
  }
  else if(chaineCommencePar(commande, "rocket.getSteps")) {
    for (int i = 0; i < NB_ETAPES; i++ ) {
      sprintf(strLog, "%i | %i ms | %s", i, dureeEtape[i], commandeEtape[i]);
      logger.log(MODULE_SYSTEME, "ROCKET_STEP", strLog);
    }
  }
  else if(chaineCommencePar(commande, "rocket.launch ") || chaineCommencePar(commande, "rocket.stage ")) {
    if(codeCorrect(commande)) {
      etapeSuivante();
    }
  }
  else if(chaineCommencePar(commande, "rocket.stop ")) {
    if(codeCorrect(commande)) {
      fuseeStatut = -1;
    }
  }
  else if(chaineCommencePar(commande, "rocket.goStep ")) {
    if(fuseeStatut > CONFIGURATION) {
      copierToken(commande, " ", 1, chEtape);
      fuseeStatut = atoi(chEtape);
      dateEtapeSuivante = millis();

      itoa(fuseeStatut, strLog, 10);
      logger.log(MODULE_SYSTEME, "JUMPING_TO_STEP", strLog);

      if(dureeEtape[fuseeStatut] != -1) {
        // La condition de passage à l'étape suivante est une durée
        dateEtapeSuivante = dateEtapeSuivante + dureeEtape[fuseeStatut];
      }
      else {
        // La condition de passage à l'étape suivante n'est pas une durée
        dateEtapeSuivante = -1;
      }
      verifierEtExecuterCommande(commandeEtape[fuseeStatut]);
    }
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

bool codeCorrect(const char commande[]) {
  bool codeCorrect = false;
  if(chaineContient(commande, SECRET_COMMAND_CODE)) {
    codeCorrect = true;
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_CODE", "Code de derrouillage de la commande incorrect");
  }
  return codeCorrect;
}

void executerCommandeServo(const char commande[]) {
  char chServo[LONGUEUR_NOMBRE];
  char chParam[LONGUEUR_NOMBRE];
  int servo;
  int param;
  if(chaineCommencePar(commande, "servo.setPosition ")) {    
    copierToken(commande, " ", 1, chServo);
    copierToken(commande, " ", 2, chParam);
    servo = atoi(chServo);
    param = atoi(chParam);
    servos[servo].write(param);
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void executerCommandeLogger(const char commande[]) {
  if(chaineCommencePar(commande, "logger.initSdcard")) {
    logger.initSdcard();
  }
  else if(chaineCommencePar(commande, "logger.flushToSdcard")) {
    logger.flush();
  }
  else if(chaineCommencePar(commande, "logger.toggleLogImuData")) {
    centrale.loggingData = !centrale.loggingData;
  }
  else if(chaineCommencePar(commande, "logger.toggleLogFlush")) {
    logger.loggingFlush = !logger.loggingFlush;
  }
  else if(chaineCommencePar(commande, "logger.toggleLogRocketStatus")) {
    loggingRocketStatus = !loggingRocketStatus;
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void executerCommandeWifi(const char commande[]) {
  char ssid[LONGUEUR_MAX_CHAINE_CARACTERES];
  char pwd[LONGUEUR_MAX_CHAINE_CARACTERES];
  if(chaineCommencePar(commande, "wifi.connect ")) {
    copierToken(commande, " ", 1, ssid);
    copierToken(commande, " ", 2, pwd);
    wifi.connecterAvecFallback(ssid, pwd);
  }
  else if(chaineCommencePar(commande, "wifi.initUdp ")) {
    // Ne rien faire : on a déjà récupéré l'IP de l'émetteur dans le module Wifi
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void executerCommandeAutre(const char commande[]) {  
  char chNombre[LONGUEUR_NOMBRE];
  int nombre;
  if (chaineCommencePar(commande, "delay ")) {
    copierToken(commande, " ", 1, chNombre);
    nombre = atoi(chNombre);
    delay(nombre);
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

