/*
 * --------------------------------------------------------------------------
* Gestion dees commandes envoyées à la fusée
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#include "Interpreteur.hpp"

const char Interpreteur::MODULE_COMMANDE[] = "COMMAND";

Interpreteur::Interpreteur(Logger &logger, Wifi &wifi, CentraleInertielle &centrale, ProgrammeVol &programmeVol, Servomoteur (&servos)[NB_SERVOS])
  :logger(logger), wifi(wifi), centrale(centrale), programmeVol(programmeVol), servos(servos) {
}


/* GESTION DES COMMANDES ENVOYEES A LA FUSEE */

void Interpreteur::lireCommande() {  
  wifi.lireUdp(commande);
  trim(commande);
  verifierEtExecuterCommande(commande);
}

void Interpreteur::verifierEtExecuterCommande(const char commande[]) {
  if(strlen(commande) > 0) {
    executerCommande(commande);
  }
}

/* Premier switch sur la classe de commandes, pour rationaliser le nombre de comparaisons
et les variables créées dans la pile */
void Interpreteur::executerCommande(const char commande[]) {
  logger.log(MODULE_COMMANDE, "EXECUTION", commande);

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

void Interpreteur::executerCommandeBroche(const char commande[]) {
  // Configuration d'une broche de l'Arduino
  if(chaineCommencePar(commande, "PC ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    copierToken(commande, " ", 2, cmdStrParam2);
    cmdIntParam1 = atoi(cmdStrParam1);
    if(chaineCommencePar(cmdStrParam2, "OUTPUT")) {
      cmdPinModeParam = OUTPUT;
    }
    else {
      cmdPinModeParam = INPUT;
    }
    pinMode(cmdIntParam1, cmdPinModeParam);
  }
  // Écriture numérique sur une broche
  else if(chaineCommencePar(commande, "PD ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    copierToken(commande, " ", 2, cmdStrParam2);
    cmdIntParam1 = atoi(cmdStrParam1);
    cmdIntParam2 = atoi(cmdStrParam2);
    digitalWrite(cmdIntParam1, cmdIntParam2);
  }
  // Sortie audio sur une broche
  else if(chaineCommencePar(commande, "PT ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    copierToken(commande, " ", 2, cmdStrParam2);
    cmdIntParam1 = atoi(cmdStrParam1);
    cmdIntParam2 = atoi(cmdStrParam2);
    tone(cmdIntParam1, cmdIntParam2);
  }
  // Arrêter le signal audio
  else if(chaineCommencePar(commande, "PR ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    cmdIntParam1 = atoi(cmdStrParam1);
    noTone(cmdIntParam1);
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void Interpreteur::executerCommandeFusee(const char commande[]) {
  // Renvoyer le statut de la fusée
  if(chaineCommencePar(commande, "RS")) {
    programmeVol.logStatutFusee();
  }
  // Passer à l'étage suivant de la fusée (uniquement indicatif)
  else if(chaineCommencePar(commande, "RE")) {    
    programmeVol.etageCourant = programmeVol.etageCourant + 1;
    itoa(programmeVol.etageCourant, strLog, 10);
    logger.log(MODULE_SYSTEME, "STAGGING", strLog);
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void Interpreteur::executerCommandePlanDeVol(const char commande[]) {
  char cmdStrParam1[LONGUEUR_NOMBRE];
  char cmdStrParam2[LONGUEUR_NOMBRE];
  int cmdIntParam1;
  int cmdIntParam2;

  // Configurer l'étape de vol
  if(chaineCommencePar(commande, "FC ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    copierToken(commande, " ", 2, cmdStrParam2);
    cmdIntParam1 = atoi(cmdStrParam1);
    cmdIntParam2 = atoi(cmdStrParam2);
    if(isNumeroEtapeValide(cmdIntParam1)) {
      programmeVol.setDureeEtape(cmdIntParam1, cmdIntParam2);
      copierToken(commande, " ", 3, programmeVol.commandeEtape[cmdIntParam1], true);
    }
  }
  // Lister ligne par ligne tout le programme de vol
  else if(chaineCommencePar(commande, "FS")) {
    for (int i = 0; i < NB_ETAPES; i++ ) {
      sprintf(strLog, "%i | %i ms | %s", i, programmeVol.getDureeEtape(i), programmeVol.commandeEtape[i]);
      logger.log(MODULE_SYSTEME, "FLIGHTPLAN_STEP", strLog);
    }
  }
  // Lancer le programme de vol (instruction 0)
  else if(chaineCommencePar(commande, "F0 ")) {
    if(codeCorrect(commande)) {
      programmeVol.lancer();
      verifierEtExecuterCommande(programmeVol.getCommandeCourante());
    }
  }
  // Arrêt d'urgence et retour à l'initialisation
  else if(chaineCommencePar(commande, "FZ ")) {
    if(codeCorrect(commande)) {
      programmeVol.stop();
    }
  }
  // Aller directement à une étape/instruction spécifique
  else if(chaineCommencePar(commande, "FG ")) {
    if(programmeVol.getEtapeCourante() > ETAPE_CONFIGURATION) {
      copierToken(commande, " ", 1, cmdStrParam1);
      cmdIntParam1 = atoi(cmdStrParam1);
      if(isNumeroEtapeValide(cmdIntParam1)) {
        programmeVol.sauterVersEtape(cmdIntParam1);
        verifierEtExecuterCommande(programmeVol.getCommandeCourante());
      }
    }
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

bool Interpreteur::codeCorrect(const char commande[]) {
  bool codeCorrect = false;
  if(chaineContient(commande, SECRET_COMMAND_CODE)) {
    codeCorrect = true;
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_CODE", "Code de déverrouillage de la commande incorrect");
  }
  return codeCorrect;
}

void Interpreteur::executerCommandeServo(const char commande[]) {
  // Servo sur un angle de 0 à 180
  if(chaineCommencePar(commande, "SA ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    copierToken(commande, " ", 2, cmdStrParam2);
    cmdIntParam1 = atoi(cmdStrParam1);
    cmdIntParam2 = atoi(cmdStrParam2);
    if(isNumeroServoValide(cmdIntParam1)
        && cmdIntParam2 >= 0 && cmdIntParam2 <= 180) {
      servos[cmdIntParam1].angle(cmdIntParam2);
        }
  }
  // Servo sur une position (avec calibration) de -100 à 100
  else if(chaineCommencePar(commande, "SP ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    copierToken(commande, " ", 2, cmdStrParam2);
    cmdIntParam1 = atoi(cmdStrParam1);
    cmdIntParam2 = atoi(cmdStrParam2);
    if(isNumeroServoValide(cmdIntParam1)
        && cmdIntParam2 >= SERVO_POSITION_MIN && cmdIntParam2 <= SERVO_POSITION_MAX) {
      servos[cmdIntParam1].position(cmdIntParam2);
    }
  }
  // Décalage d'offset pour un servo
  else if(chaineCommencePar(commande, "SO ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    copierToken(commande, " ", 2, cmdStrParam2);
    cmdIntParam1 = atoi(cmdStrParam1);
    cmdIntParam2 = atoi(cmdStrParam2);
    if(isNumeroServoValide(cmdIntParam1)) {
      servos[cmdIntParam1].setDecalage(cmdIntParam2);
    }
  }
  // Coefficient entre angle et position
  else if(chaineCommencePar(commande, "SS ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    copierToken(commande, " ", 2, cmdStrParam2);
    cmdIntParam1 = atoi(cmdStrParam1);
    cmdFloatParam1 = atof(cmdStrParam2);
    if(isNumeroServoValide(cmdIntParam1)) {
      servos[cmdIntParam1].setPente(cmdFloatParam1);
    }
  }
  // Paramètres et consigne actuelle du servo
  else if(chaineCommencePar(commande, "SR ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    cmdIntParam1 = atoi(cmdStrParam1);
    if(isNumeroServoValide(cmdIntParam1)) {
      servos[cmdIntParam1].logStatut();
    }
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void Interpreteur::executerCommandeLogger(const char commande[]) {
  // Initialiser carte SD
  if(chaineCommencePar(commande, "LC")) {
    logger.initCarteSd();
  }
  // Écriture d'un log quelconque
  else if(chaineCommencePar(commande, "LL")) {
    copierToken(commande, " ", 1, cmdStrParam1, true);
    logger.log(MODULE_COMMANDE, "CUSTOM_LOG", cmdStrParam1);
  }
  // Forcer l'écriture sur la carte SD
  else if(chaineCommencePar(commande, "LF")) {
    logger.forcerEcritureSurCarteSd();
  }
  // Renvoie la taille du fichier de logs
  else if(chaineCommencePar(commande, "LT")) {
    logger.tailleFichierCarteSd();
  }
  // Vide le fichier de logs
  else if(chaineCommencePar(commande, "LD")) {
    logger.viderFichierCarteSd();
  }
  // Statuts des loggers
  else if(chaineCommencePar(commande, "LS")) {
    logger.logStatut();
  }
  // Nombre d'échantillons pour les logs des données inertielles. 0 => pas de log.
  else if(chaineCommencePar(commande, "LI")) {    
    copierToken(commande, " ", 1, cmdStrParam1);
    cmdIntParam1 = atoi(cmdStrParam1);
    centrale.samplesByDataLog = cmdIntParam1;
  }
  // Désactiver les logs des flushes SD
  else if(chaineCommencePar(commande, "LW 0")) {
    logger.logForcageEcritureSurCarteSd = false;
  }
  // Activer les logs des flushes SD
  else if(chaineCommencePar(commande, "LW 1")) {
    logger.logForcageEcritureSurCarteSd = true;
  }
  // Désactiver les logs d'états de la fusée
  else if(chaineCommencePar(commande, "LR 0")) {
    programmeVol.loggingStatutFusee = false;
  }
  // Activer les logs d'états de la fusée
  else if(chaineCommencePar(commande, "LR 1")) {
    programmeVol.loggingStatutFusee = true;
  }
  // Désactiver les logs vers la liaison série
  else if(chaineCommencePar(commande, "LX 0")) {
    logger.logSerieActif = false;
  }
  // Activer les logs vers la liaison série
  else if(chaineCommencePar(commande, "LX 1")) {
    logger.logSerieActif = true;
  }
  // Désactiver les logs vers la carte SD
  else if(chaineCommencePar(commande, "LY 0")) {
    logger.logCarteSdActif = false;
  }
  // Activer les logs vers la carte SD
  else if(chaineCommencePar(commande, "LY 1")) {
    logger.logCarteSdActif = true;
  }
  // Désactiver les logs via le wifi
  else if(chaineCommencePar(commande, "LZ 0")) {
    logger.logUdpActif = false;
  }
  // Activer les logs via le wifi
  else if(chaineCommencePar(commande, "LZ 1")) {
    logger.logUdpActif = true;
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void Interpreteur::executerCommandeWifi(const char commande[]) {
  // Se connecter à un réseau WiFi
  if(chaineCommencePar(commande, "WC ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    copierToken(commande, " ", 2, cmdStrParam2);
    wifi.connecterAvecFallback(cmdStrParam1, cmdStrParam2);
  }
  // Statut du WiFi
  else if(chaineCommencePar(commande, "WS")) {
    wifi.logStatut();
  }
  // Enregistrer un nouveau PC connecté à la fusée
  else if(chaineCommencePar(commande, "WB ")) {
    // On a déjà récupéré l'IP et le port de l'émetteur dans le module Wifi
    // On se contente d'envoyer le numéro de client
    copierToken(commande, " ", 1, cmdStrParam1); // n° client UDP
    // TODO Vérifier nombre
    wifi.confClientUdp(atoi(cmdStrParam1));
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void Interpreteur::executerCommandeCentraleInertielle(const char commande[]) {  
  // Renvoie toutes les données inertielles contenues dans le buffer de calibration
  if(chaineCommencePar(commande, "IB")) {
    centrale.logDonneesCalibration();
  }
  // Intégre des offsets dans les données brutes des capteurs de façon à avoir exactement les valeurs passées en calibration
  else if(chaineCommencePar(commande, "IC ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    copierToken(commande, " ", 2, cmdStrParam2);
    copierToken(commande, " ", 3, cmdStrParam3);
    copierToken(commande, " ", 4, cmdStrParam4);
    copierToken(commande, " ", 5, cmdStrParam5);
    copierToken(commande, " ", 6, cmdStrParam6);
    centrale.calibrer(atof(cmdStrParam1), atof(cmdStrParam2), atof(cmdStrParam3), atof(cmdStrParam4), atof(cmdStrParam5), atof(cmdStrParam6));
  }
  // Paramètre l'accélération minimale
  else if(chaineCommencePar(commande, "IACC ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    centrale.setFiltreMinAcceleration(atof(cmdStrParam1));
  }
  // Paramètre la vitesse angulaire minimale
  else if(chaineCommencePar(commande, "IANG ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    centrale.setFiltreMinVitesseAngulaire(atof(cmdStrParam1));
  }
  
  // Activition de la stabilisation par la tuyère orientable
  else if(chaineCommencePar(commande, "IR 1")) {
    centrale.rcsActif = true;
  }
  // Désactivition de la stabilisation par la tuyère orientable
  else if(chaineCommencePar(commande, "IR 0")) {
    centrale.rcsActif = false;
  }
  // Servo contrôlant l'axe X pour la stabilisation par la tuyère orientable
  else if(chaineCommencePar(commande, "IU ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    cmdIntParam1 = atoi(cmdStrParam1);
    if(isNumeroServoValide(cmdIntParam1)) {
      centrale.setRcsServoX(&servos[cmdIntParam1]);
    }
  }
  // Servo contrôlant l'axe Y pour la stabilisation par la tuyère orientable
  else if(chaineCommencePar(commande, "IV ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    cmdIntParam1 = atoi(cmdStrParam1);
    if(isNumeroServoValide(cmdIntParam1)) {
      centrale.setRcsServoY(&servos[cmdIntParam1]);
    }
  }
  
  // Activition de la stabilisation par les ailerons
  else if(chaineCommencePar(commande, "IW 1")) {
    centrale.wcsActif = true;
  }
  // Désactivition de la stabilisation par les ailerons
  else if(chaineCommencePar(commande, "IW 0")) {
    centrale.wcsActif = false;
  }
  // Servo contrôlant l'axe X pour la stabilisation par les ailerons
  else if(chaineCommencePar(commande, "IX ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    cmdIntParam1 = atoi(cmdStrParam1);
    if(isNumeroServoValide(cmdIntParam1)) {
      centrale.setWcsServoX(&servos[cmdIntParam1]);
    }
  }
  // Servo contrôlant l'axe Y pour la stabilisation par les ailerons
  else if(chaineCommencePar(commande, "IY ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    cmdIntParam1 = atoi(cmdStrParam1);
    if(isNumeroServoValide(cmdIntParam1)) {
      centrale.setWcsServoY(&servos[cmdIntParam1]);
    }
  }
  // Paramètres de la fonction de correction (asservissement de la trajectoire)
  else if(chaineCommencePar(commande, "ICF ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    copierToken(commande, " ", 2, cmdStrParam2);
    copierToken(commande, " ", 3, cmdStrParam3);
    copierToken(commande, " ", 4, cmdStrParam4);
    centrale.setCorrectionFunctionParameters(atof(cmdStrParam1), atof(cmdStrParam2), atof(cmdStrParam3), atof(cmdStrParam4));
  }

  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}

void Interpreteur::executerCommandeAutre(const char commande[]) {  
  // Délai
  if (chaineCommencePar(commande, "0D ")) {
    copierToken(commande, " ", 1, cmdStrParam1);
    cmdIntParam1 = atoi(cmdStrParam1);
    delay(cmdIntParam1);
  }
  else {
    logger.log(MODULE_COMMANDE, "COMMAND_ERROR_UNKNOWN", "Commande non reconnue");
  }
}



bool Interpreteur::isNumeroEtapeValide(int etape) {
  return etape >= 0 && etape < NB_ETAPES;
}

bool Interpreteur::isNumeroServoValide(int servo) {
  return servo >= 0 && servo < NB_SERVOS;
}