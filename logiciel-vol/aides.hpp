/*
* Quelques fonctions d'aide
*/

#ifndef aides_h
#define aides_h

#include <Arduino.h>

// Constantes générales
inline constexpr char SEPARATEUR[] = ", ";
inline constexpr char GUILLEMETS[] = "\"";
inline constexpr int LONGUEUR_MAX_CHAINE_CARACTERES = 256;
inline constexpr int LONGUEUR_ADRESSE_IP = 20;

// Formatage des log
void formatLog(char destination[], const char module[], const char message[], const char details[]);

#endif