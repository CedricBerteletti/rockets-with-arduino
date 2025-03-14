/*
 * --------------------------------------------------------------------------
 * Quelques fonctions utiles sur les chaînes de caractères
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#ifndef aides_h
#define aides_h

#include <Arduino.h>

// Formatage des log
void formaterLog(char destination[], const char module[], const char message[], const char details[]);

// Teste si une chaîne de caractères commence par un prefixe donné
bool chaineCommencePar(const char chaine[], const char prefix[]);

// Teste si une chaîne de caractères contient une sous-chaîne
bool chaineContient(const char chaine[], const char sousChaine[]);

// Supprime les espaces au début et à la fin de la chaîne de caractères
// https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
char *trim(char *str);

// Copie une sous-chaîne à l'index donné de la chaîne principale, comprise entre des séparateurs (si dernierToken false),
// ou comprise entre un séparateur et la fin (si dernierToken vrai)
void copierToken(const char chaine[], const char separateur[], int index, char *token, bool dernierToken);
// Copie une sous-chaîne à l'index donné de la chaîne principale, comprise entre des séparateurs
void copierToken(const char chaine[], const char separateur[], int index, char *token);

// Flash lumineux sur la broche pin
void flash(int pin);
void flash(int pin, int nombre);

#endif