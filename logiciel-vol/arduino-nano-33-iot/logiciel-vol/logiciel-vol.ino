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

// Nombre d'étapes/instructions du programme de vol
static const int NB_ETAPES = 100;

static const char MODULE_SYSTEME[] = "SYSTEM";
static const char MODULE_COMMANDE[] = "COMMAND";
static const long DELAI_FLUSH = 1000; // (en ms) Flush des logs sur la carte SD toutes les secondes
static const long DELAI_COMMANDE = 100; // (en ms) Tentative de lecture d'une nouvelle commande toutes les 100 ms

/* Variable globales */
// Instanciation d'un loggeur
Logger logger;
bool loggingStatutFusee = true;

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

// Servomoteurs des ailerons et de la tuyère sur les broches 18 à 21
static const int NB_SERVOS = 4;
Servo servos[NB_SERVOS];
static const int SERVO0_PIN = 6;
static const int SERVO1_PIN = 7;
static const int SERVO2_PIN = 20;
static const int SERVO3_PIN = 21;

/* Etats de la fusée */
static const int CONFIGURATION = -1;
static const int DECOMPTE_FINAL = 0;
int etage = 0;

/* Programme de vol */
int etape = CONFIGURATION;
int dureeEtape[NB_ETAPES];
char commandeEtape[NB_ETAPES][LONGUEUR_MAX_COMMANDE];
long dateLancement = 0;
long dateEtapeSuivante = -1;
long dateCourante = 0;

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

  // 4 servomoteurs sur les broches 18 à 21
  servos[0].attach(SERVO0_PIN);
  servos[1].attach(SERVO1_PIN);
  servos[2].attach(SERVO2_PIN);
  servos[3].attach(SERVO3_PIN);

  delay(1000);
  dateCourante = millis();
  dateFlushSuivant = dateCourante;
  dateLectureCommandeSuivante = dateCourante;
}


// Boucle principale : lecture des capteurs ou des commandes et actions
void loop() {
  dateCourante = millis();
  centrale.lire();

  // Actions à fréquence réduite (pas effectuées à chaque itération de loop())
  if(dateCourante > dateFlushSuivant) {
    logger.flush();
    dateFlushSuivant = dateFlushSuivant + DELAI_FLUSH;
    if(loggingStatutFusee) {
      logStatutFusee();
    }
  }
  if(dateCourante > dateLectureCommandeSuivante) {
    dateLectureCommandeSuivante = dateLectureCommandeSuivante + DELAI_COMMANDE;
    lireCommande();
  }

  if(dateEtapeSuivante != -1 && dateCourante > dateEtapeSuivante) {
    // On passe à l'instruction suivante dans le programme de vol
    etapeSuivante();
  }
}

/* Initialisation de la liaison série (pour débogage sur un PC) */
void initSerial() {
  Serial.begin(115200);
  logger.log(MODULE_SYSTEME, "SERIAL_INIT", "Liaison série initialisée");
}

/* Log l'état/étape/étage actuel de la fusée */
void logStatutFusee() {
  if(etape > CONFIGURATION) {
    // t = 0s commence lorsque l'on arrive à l'instruction/étape 1
    int dateEnSeconde = (dateCourante - dateLancement - dureeEtape[0])/1000;
    sprintf(strLog, "t=%is | Étape %i | Étage %i", dateEnSeconde, etape, etage);
  }
  else {
    sprintf(strLog, "En attente | Étape %i | Étage %i", etape, etage);
  }
  logger.log(MODULE_SYSTEME, "ROCKET_STATUS", strLog);
}

/* La fusée passe l'instruction suivante du pprogramme de vol
  - on incrémente le pointeur d'étape/instruction de la fusée (initalement à -1)
  - on exécute la commande correspondant à cette nouvelle étape
  - on indique la date de la prochaine transition d'étage
*/
void etapeSuivante() {
  etape = etape + 1;
  if(etape > CONFIGURATION) {
    if(etape == DECOMPTE_FINAL) {
      // Première étape : le décompte final ! La fusée est en auto à partir de maintenant
      // (ie, transition d'état selon ce qui a été précédemment paramétré)
      dateLancement = millis();
      // On commence à afficher les logs sur les 
      loggingStatutFusee = true;
      // Initialisation de la dateEtapeSuivante à la date courante
      dateEtapeSuivante = dateLancement;
    }

    itoa(etape, strLog, 10);
    logger.log(MODULE_SYSTEME, "STEP", strLog);

    executerEtapeEtPreparerEtapeSuivante();
  }
}

void executerEtapeEtPreparerEtapeSuivante() {
  if(dureeEtape[etape] != -1) {
    // La condition de passage à l'étape suivante est une durée
    dateEtapeSuivante = dateEtapeSuivante + dureeEtape[etape];
  }
  else {
    // La condition de passage à l'étape suivante n'est pas une durée
    dateEtapeSuivante = -1;
  }
  verifierEtExecuterCommande(commandeEtape[etape]);
}

/* GESTION DES COMMANDES ENVOYEES A LA FUSEE */

void lireCommande() {  
  wifi.lireUdp(commande);
  trim(commande);
  verifierEtExecuterCommande(commande);
}

void verifierEtExecuterCommande(const char commande[]) {
  if(strlen(commande) > 0) {
    Serial.println ("Lire commande");
    executerCommande(commande);
  }
}

/* Premier switch sur la classe de commandes, pour rationaliser le nombre de comparaisons
et les variables créées dans la pile */
void executerCommande(const char commande[]) {
  logger.log(MODULE_COMMANDE, "COMMAND_RECEPTION", commande);

  if(chaineCommencePar(commande, "P")) {
    executerCommandeBroche(commande);
  }
  else if(chaineCommencePar(commande, "R")) {
    executerCommandeFusee(commande);
  }
  else if(chaineCommencePar(commande, "F")) {
    executerCommandePlanDeVol(commande);
  }
  else if(chaineCommencePar(commande, "I")) {
    executerCommandeCentraleInertielle(commande);
  }
  else if(chaineCommencePar(commande, "S")) {
    executerCommandeServo(commande);
  }
  else if(chaineCommencePar(commande, "L")) {
    executerCommandeLogger(commande);
  }
  else if(chaineCommencePar(commande, "W")) {
    executerCommandeWifi(commande);
  }
  else if(chaineCommencePar(commande, "#")) {
    // Ne rien faire : ligne de commentaire
  }
  else {
    executerCommandeAutre(commande);
  }
}

void executerCommandeBroche(const char commande[]) {
  char chPin[LONGUEUR_NOMBRE];
  char chParam[LONGUEUR_NOMBRE];
  int pin;
  int param;

  // Configuration d'une broche de l'Arduino
  if(chaineCommencePar(commande, "PC ")) {
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
  // Écriture numérique sur une broche
  else if(chaineCommencePar(commande, "PD ")) {
    copierToken(commande, " ", 1, chPin);
    copierToken(commande, " ", 2, chParam);
    pin = atoi(chPin);
    param = atoi(chParam);
    digitalWrite(pin, param);
  }
  // Sortie audio sur une broche
  else if(chaineCommencePar(commande, "PT ")) {
    copierToken(commande, " ", 1, chPin);
    copierToken(commande, " ", 2, chParam);
    pin = atoi(chPin);
    param = atoi(chParam);
    tone(pin, param);
  }
  // Arrêter le signal audio
  else if(chaineCommencePar(commande, "PR ")) {
    copierToken(commande, " ", 1, chPin);
    pin = atoi(chPin);
    noTone(pin);
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void executerCommandeFusee(const char commande[]) {
  char chEtape[LONGUEUR_NOMBRE];
  char chDelai[LONGUEUR_NOMBRE];
  int etape;
  int delai;

  // Renvoyer le statut de la fusée
  if(chaineCommencePar(commande, "RS")) {
    logStatutFusee();
  }
  // Passer à l'étage suivant de la fusée (uniquement indicatif)
  else if(chaineCommencePar(commande, "RE")) {    
    etage = etage + 1;
    itoa(etape, strLog, 10);
    logger.log(MODULE_SYSTEME, "STAGGING", strLog);
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void executerCommandePlanDeVol(const char commande[]) {
  char chEtape[LONGUEUR_NOMBRE];
  char chDelai[LONGUEUR_NOMBRE];
  int paramEtape;
  int paramDelai;

  // Configurer l'étape de vol
  if(chaineCommencePar(commande, "FC ")) {
    copierToken(commande, " ", 1, chEtape);
    copierToken(commande, " ", 2, chDelai);
    paramEtape = atoi(chEtape);
    paramDelai = atoi(chDelai);
    dureeEtape[paramEtape] = paramDelai;
    // TODO Vérifier paramEtape
    copierToken(commande, " ", 3, commandeEtape[paramEtape], true);
  }
  // Lister ligne par ligne tout le programme de vol
  else if(chaineCommencePar(commande, "FS")) {
    for (int i = 0; i < NB_ETAPES; i++ ) {
      sprintf(strLog, "%i | %i ms | %s", i, dureeEtape[i], commandeEtape[i]);
      logger.log(MODULE_SYSTEME, "FLIGHTPLAN_STEP", strLog);
    }
  }
  // Lancer le programme de vol (instruction 0)
  else if(chaineCommencePar(commande, "F0 ")) {
    if(codeCorrect(commande)) {
      etape = -1;
      etage = 0;
      dateEtapeSuivante = -1;
      etapeSuivante();
    }
  }
  // Arrêt d'urgence et retour à l'initialisation
  else if(chaineCommencePar(commande, "FZ ")) {
    if(codeCorrect(commande)) {
      etape = -1;
      etage = 0;
      dateEtapeSuivante = -1;
    }
  }
  // Aller directement à une étape/instruction spécifique
  else if(chaineCommencePar(commande, "FG ")) {
    if(etape > CONFIGURATION) {
      copierToken(commande, " ", 1, chEtape);
      // TODO Vérifier chEtape
      etape = atoi(chEtape);
      dateEtapeSuivante = millis();

      itoa(etape, strLog, 10);
      logger.log(MODULE_SYSTEME, "GOTO_STEP", strLog);

      executerEtapeEtPreparerEtapeSuivante();
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
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_CODE", "Code de déverrouillage de la commande incorrect");
  }
  return codeCorrect;
}

void executerCommandeServo(const char commande[]) {
  char chServo[LONGUEUR_NOMBRE];
  char chParam[LONGUEUR_NOMBRE];
  int servo;
  int param;

  // Servo sur un angle de 0 à 180
  if(chaineCommencePar(commande, "SA ")) {
    copierToken(commande, " ", 1, chServo);
    copierToken(commande, " ", 2, chParam);
    servo = atoi(chServo);
    param = atoi(chParam);
    // TODO Vérifier servo
    servos[servo].write(param);
  }// Servo sur une position (avec calibration) de 0 à 100
  else if(chaineCommencePar(commande, "SP ")) {
    copierToken(commande, " ", 1, chServo);
    copierToken(commande, " ", 2, chParam);
    servo = atoi(chServo);
    param = atoi(chParam);
    // TODO Vérifier servo
    // TODO
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void executerCommandeLogger(const char commande[]) {
  // Initialiser carte SD
  if(chaineCommencePar(commande, "LC")) {
    logger.initSdcard();
  }
  // Forcer l'écriture sur la carte SD
  else if(chaineCommencePar(commande, "LF")) {
    logger.flush();
  }
  // Statuts des loggers
  else if(chaineCommencePar(commande, "LS")) {
    logger.logStatut();
  }
  // Désactiver les logs des données inertielles
  else if(chaineCommencePar(commande, "LI 0")) {
    centrale.loggingData = false;
  }
  // Activer les logs des données inertielles
  else if(chaineCommencePar(commande, "LI 1")) {
    centrale.loggingData = true;
  }
  // Désactiver les logs des flushes SD
  else if(chaineCommencePar(commande, "LW 0")) {
    logger.loggingFlush = false;
  }
  // Activer les logs des flushes SD
  else if(chaineCommencePar(commande, "LW 1")) {
    logger.loggingFlush = true;
  }
  // Désactiver les logs d'états de la fusée
  else if(chaineCommencePar(commande, "LR 0")) {
    loggingStatutFusee = false;
  }
  // Activer les logs d'états de la fusée
  else if(chaineCommencePar(commande, "LR 1")) {
    loggingStatutFusee = true;
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void executerCommandeWifi(const char commande[]) {
  char ssid[LONGUEUR_MAX_CHAINE_CARACTERES];
  char pwd[LONGUEUR_MAX_CHAINE_CARACTERES];
  char nombre[LONGUEUR_NOMBRE];

  // Se connecter à un réseau WiFi
  if(chaineCommencePar(commande, "WC ")) {
    copierToken(commande, " ", 1, ssid);
    copierToken(commande, " ", 2, pwd);
    wifi.connecterAvecFallback(ssid, pwd);
  }
  // Statut du WiFi
  else if(chaineCommencePar(commande, "WS")) {
    wifi.logStatut();
  }
  // Enregistrer un nouveau PC connecté à la fusée
  else if(chaineCommencePar(commande, "WB ")) {
    // On a déjà récupéré l'IP et le port de l'émetteur dans le module Wifi
    // On se contente d'envoyer le numéro de client
    copierToken(commande, " ", 1, nombre); // n° client UDP
    // TODO Vérifier nombre
    wifi.confClientUdp(atoi(nombre));
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void executerCommandeCentraleInertielle(const char commande[]) {
  char ax[LONGUEUR_NOMBRE];
  char ay[LONGUEUR_NOMBRE];
  char az[LONGUEUR_NOMBRE];
  char valpha[LONGUEUR_NOMBRE];
  char vbeta[LONGUEUR_NOMBRE];
  char vgamma[LONGUEUR_NOMBRE];
  
  // Renvoie toutes les données inertielles contenues dans le buffer de calibration
  if(chaineCommencePar(commande, "IB")) {
    centrale.logBuffer();
  }
  // Intégre des offsets dans les données brutes des capteurs de façon à avoir exactement les valeurs passées en calibration
  else if(chaineCommencePar(commande, "IC ")) {
    // TODO
  }
  // Paramètre l'accélération minimale
  else if(chaineCommencePar(commande, "IA ")) {
    // TODO
  }
  // Paramètre la vitesse angulaire minimale
  else if(chaineCommencePar(commande, "IV ")) {
    // TODO
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void executerCommandeAutre(const char commande[]) {  
  char chNombre[LONGUEUR_NOMBRE];
  int nombre;

  // Délai
  if (chaineCommencePar(commande, "0D ")) {
    copierToken(commande, " ", 1, chNombre);
    nombre = atoi(chNombre);
    delay(nombre);
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

