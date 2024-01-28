/*
 * --------------------------------------------------------------------------
 * Gestion d'un servomoteur
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#ifndef Servomoteur_h
#define Servomoteur_h

#include <Arduino.h>
#include "Logger.hpp"
#include <Servo.h>

class Servomoteur
{
  public:
    Servomoteur();
    void init(int id, int broche, Logger *logger);
    void setDecalage(int decalage);
    void setPente(float pente);
    void angle(int angle);
    void position(int position);
    void logStatut();
  
  private:
    static const char MODULE_SERVO[];
    char strTmp[LONGUEUR_MAX_CHAINE_CARACTERES];
    Logger *logger;
    Servo servo;
    int id;
    int broche;
    int decalage = 0;
    float pente = 1.0f;    
};

#endif