/*
 * --------------------------------------------------------------------------
 * Programme de vol de la fusée
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */


#include "ProgrammeVol.hpp"


ProgrammeVol::ProgrammeVol(Logger &logger)
  :logger(logger) {
  // Blocage des transitions automatiques
  for (int i = 0; i < NB_ETAPES; i++ ) {
    dureeEtape[i] = -1;
  }
}


void ProgrammeVol::init(long date) {
  dateCourante = date;
}

// Lancer le programme de vol (instruction 0)
void ProgrammeVol::lancer() {
  etape = -1;
  etageCourant = 0;
  dateEtapeSuivante = -1;
  etapeSuivante();
}

void ProgrammeVol::stop() {
  etape = -1;
  etageCourant = 0;
  dateEtapeSuivante = -1;
}

bool ProgrammeVol::evaluerEtapeSuivante() {
  bool etapePrete = false;
  if(dateEtapeSuivante != -1 && dateCourante > dateEtapeSuivante) {
    // On passe à l'instruction suivante dans le programme de vol
    etapeSuivante();
    etapePrete = true;
  }
  return etapePrete;
}

/* La fusée passe l'instruction suivante du pprogramme de vol
  - on incrémente le pointeur d'étape/instruction de la fusée (initalement à -1)
  - on exécute la commande correspondant à cette nouvelle étape
  - on indique la date de la prochaine transition d'étage
*/
void ProgrammeVol::etapeSuivante() {
  etape = etape + 1;
  if(etape > ETAPE_CONFIGURATION) {
    if(etape == ETAPE_DECOMPTE_FINAL) {
      // Étapes d'initialisation jusqu'au décompte final ! La fusée est en auto à partir de maintenant
      // (ie, transition d'état selon ce qui a été précédemment paramétré)
      dateLancement = millis();
      dureeCompteARebours = 0;
      for (int i = ETAPE_CONFIGURATION+1; i <= ETAPE_DECOMPTE_FINAL; i++ ) {
        dureeCompteARebours = dureeCompteARebours + dureeEtape[i];
      }

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

// Aller directement à une étape/instruction spécifique
void ProgrammeVol::sauterVersEtape(int e) {
  etape = e;
  dateEtapeSuivante = millis();

  itoa(etape, strLog, LONGUEUR_NOMBRE);
  logger.log(MODULE_SYSTEME, "GOTO_STEP", strLog);

  executerEtapeEtPreparerEtapeSuivante();
}

void ProgrammeVol::executerEtapeEtPreparerEtapeSuivante() {
  if(dureeEtape[etape] != -1) {
    // La condition de passage à l'étape suivante est une durée
    dateEtapeSuivante = dateEtapeSuivante + dureeEtape[etape];
  }
  else {
    // La condition de passage à l'étape suivante n'est pas une durée
    dateEtapeSuivante = -1;
  }
}

void ProgrammeVol::logStatutFusee() {
  if(etape > ETAPE_CONFIGURATION) {
    // t = 0s commence lorsque l'on arrive à l'instruction/étape ETAPE_DECOMPTE_FINAL
    int dateEnSeconde = dateCourante - dateLancement - dureeCompteARebours/1000;
    sprintf(strLog, "t=%is | Étape %i | Étage %i", dateEnSeconde, etape, etageCourant);
  }
  else {
    sprintf(strLog, "En attente | Étape %i | Étage %i", etape, etageCourant);
  }
  logger.log(MODULE_SYSTEME, "ROCKET_STATUS", strLog);
}

int ProgrammeVol::getEtapeCourante() {
  return etape;
}

long ProgrammeVol::getDureeEtape(int etape) {
  return dureeEtape[etape];
}

void ProgrammeVol::setDureeEtape(int etape, long duree) {
  dureeEtape[etape] = duree;
}

char *ProgrammeVol::getCommandeCourante() {
  return commandeEtape[getEtapeCourante()];
}

