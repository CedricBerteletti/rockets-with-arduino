/*
 * --------------------------------------------------------------------------
 * Programme de vol de la fusée
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#ifndef ProgrammeVol_h
#define ProgrammeVol_h


#include "constants.h"
#include "Logger.hpp"

class ProgrammeVol
{
  public:
    ProgrammeVol(Logger &logger);
    bool loggingStatutFusee = true;
    // Chaîne de caractères pour les logs
    char strLog[LONGUEUR_MAX_CHAINE_CARACTERES];

    int etageCourant = 0;  // Uniquement indicatif
    long dateCourante = 0;
    char commandeEtape[NB_ETAPES][LONGUEUR_MAX_COMMANDE];

    void init(long date);
    void lancer();
    void stop();
    bool evaluerEtapeSuivante();
    void etapeSuivante();
    void logStatutFusee();
    void sauterVersEtape(int etape); // Aller directement à une étape/instruction spécifique

    int getEtapeCourante();
    int getDureeEtape(int etape);
    void setDureeEtape(int etape, int duree);
    char *getCommandeCourante();

  private:
    Logger &logger;

    int etape = ETAPE_CONFIGURATION;
    long dateLancement = 0;
    long dateEtapeSuivante = -1;
    int dureeEtape[NB_ETAPES];
    
    void executerEtapeEtPreparerEtapeSuivante();
};


#endif