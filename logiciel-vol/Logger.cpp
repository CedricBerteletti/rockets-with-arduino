/*
* Logger
*/

#include "Logger.h"

Logger::Logger() {}

// Fonction d'écriture des logs
void Logger::log(String module, String message, String details) {
  const String SEPARATEUR = ", ";
  const String GUILLEMETS = "\"";

  String log;
  if (toSerial || toSdcard || toUdp) {
    log = String(millis() + SEPARATEUR
      + module + SEPARATEUR
      + GUILLEMETS + message + GUILLEMETS + SEPARATEUR
      + GUILLEMETS + details + GUILLEMETS);
  }

  if(toSerial) Serial.println(log);
}
