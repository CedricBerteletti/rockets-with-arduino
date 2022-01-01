/*
* Quelques fonctions d'aide
*/

#include "aides.h"

// Formatage des log
// String formatLog(String module, String message, String details) {

//   return String(millis() + SEPARATEUR
//       + module + SEPARATEUR
//       + GUILLEMETS + message + GUILLEMETS + SEPARATEUR
//       + GUILLEMETS + details + GUILLEMETS);
// }

// Formatage des log
void formatLog(char module[], char message[], char details[], char destination[]) {
  sprintf(destination, "%i, %s, \"%s\", \"%s\"", millis(), module, message, details);
  // strcopy(destination, str2);
  // strcat(str1, str2);

  // return String(millis() + SEPARATEUR
  //     + module + SEPARATEUR
  //     + GUILLEMETS + message + GUILLEMETS + SEPARATEUR
  //     + GUILLEMETS + details + GUILLEMETS);
}
