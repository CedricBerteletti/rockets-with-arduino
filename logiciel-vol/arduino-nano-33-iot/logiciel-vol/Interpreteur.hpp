/*
 * --------------------------------------------------------------------------
 * Gestion des commandes envoyées à la fusée
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#ifndef Interpreteur_h
#define Interpreteur_h

#include <Arduino.h>
#include "constants.h"
#include "aides.hpp"
#include "Logger.hpp"
#include "Wifi.hpp"
#include "CentraleInertielle.hpp"
#include "Servomoteur.hpp"
#include "ProgrammeVol.hpp"

class Interpreteur
{
  public:
    Interpreteur(Logger &logger, Wifi &wifi, CentraleInertielle &centrale, ProgrammeVol &programmeVol, Servomoteur (&servos)[NB_SERVOS]);
    void lireCommande();
    void verifierEtExecuterCommande(const char commande[]);
    /* Premier switch sur la classe de commandes, pour rationaliser le nombre de comparaisons
    et les variables créées dans la pile */
    void executerCommande(const char commande[]);

  private:
    static const char MODULE_COMMANDE[];

    void executerCommandeBroche(const char commande[]);
    void executerCommandeFusee(const char commande[]);
    void executerCommandePlanDeVol(const char commande[]);
    bool codeCorrect(const char commande[]);
    void executerCommandeServo(const char commande[]);
    void executerCommandeLogger(const char commande[]);
    void executerCommandeWifi(const char commande[]);
    void executerCommandeCentraleInertielle(const char commande[]);
    void executerCommandeAutre(const char commande[]);

    bool isNumeroEtapeValide(int etape);
    bool isNumeroServoValide(int servo);

    Logger &logger;
    Wifi &wifi;
    CentraleInertielle &centrale;
    ProgrammeVol &programmeVol;
    Servomoteur (&servos)[NB_SERVOS];

    // Chaîne de caractères pour les logs
    char strLog[LONGUEUR_MAX_CHAINE_CARACTERES];

    // Dernière commande reçue (déclaration globale pour éviter la réallocation dans la pile à chaque appel de lireCommande)
    char commande[LONGUEUR_MAX_COMMANDE];
    // Variables pour l'interprétation des commandes
    // (variables globales => pas d'empilage/dépilage lors des appels de fonction et meilleures perf en embarqué)
    char cmdStrParam1[LONGUEUR_MAX_COMMANDE];
    char cmdStrParam2[LONGUEUR_MAX_COMMANDE];
    char cmdStrParam3[LONGUEUR_MAX_COMMANDE];
    char cmdStrParam4[LONGUEUR_MAX_COMMANDE];
    char cmdStrParam5[LONGUEUR_MAX_COMMANDE];
    char cmdStrParam6[LONGUEUR_MAX_COMMANDE];
    int cmdIntParam1;
    int cmdIntParam2;
    int cmdFloatParam1;
    PinMode cmdPinModeParam;
};

#endif

