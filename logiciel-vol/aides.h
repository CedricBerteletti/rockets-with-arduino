/*
* Quelques fonctions d'aide
*/

#ifndef aides_h
#define aides_h

#include "Arduino.h"

const char SEPARATEUR[] = ", ";
const char GUILLEMETS[] = "\"";
const int STRING_MAX_LENGTH = 256;

// Formatage des log
//String formatLog(String module, String message, String details);
void formatLog(char module[], char message[], char details[], char destination[]);

#endif