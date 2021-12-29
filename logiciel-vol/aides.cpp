/*
* Quelques fonctions d'aide
*/

#include "aides.h"

// Formatage des log
String formatLog(String module, String message, String details) {
  const String SEPARATEUR = ", ";
  const String GUILLEMETS = "\"";

  return String(millis() + SEPARATEUR
      + module + SEPARATEUR
      + GUILLEMETS + message + GUILLEMETS + SEPARATEUR
      + GUILLEMETS + details + GUILLEMETS);
}