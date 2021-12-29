/*
* Logger
*/

#include "Logger.h"

Logger::Logger() {}

// Fonction d'écriture des logs
void Logger::log(String module, String message, String details) {
  String log;
  if (toSerial || toSdcard || toUdp) {
    log = formatLog(module, message, details);
  }

  if(toSerial) Serial.println(log);
  if(toUdp) wifi->ecrireUdp(log);
}
