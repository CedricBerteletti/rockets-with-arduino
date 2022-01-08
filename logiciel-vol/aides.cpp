/*
* Quelques fonctions d'aide
*/

#include "aides.hpp"

// Formatage des log
void formatLog(char destination[], const char module[], const char message[], const char details[]) {
  sprintf(destination, "%i, %s, \"%s\", \"%s\"", millis(), module, message, details);
}
