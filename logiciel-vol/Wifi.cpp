/*
* Module gérant les connexions wifi
*/

#include "Wifi.h"

const String Wifi::MODULE_WIFI = "WIFI";

Wifi::Wifi(Logger &logger):logger(logger) {

}

void Wifi::listerReseaux()
{
  logger.log(MODULE_WIFI, "Recherche ...", "");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    logger.log(MODULE_WIFI, "Aucun réseau trouvé", "");
  }
  else {
    logger.log(MODULE_WIFI, "Nombre de réseaux trouvés : ", String(numSsid));

    for (int net = 0; net < numSsid; net++) {
      logger.log(MODULE_WIFI, "Réseau Wifi trouvé : ", String(String(net)
        + " | " + String(WiFi.SSID(net))
        + " | Signal : " + String(WiFi.RSSI(net)) + " dBm")
        + " | " + typeChiffrement(WiFi.encryptionType(net)) );
    }
  }
}

void Wifi::init(char ssid[], char pwd[])
{
  int statut = WL_IDLE_STATUS;
  long tempsDebut = millis();

  while (statut != WL_CONNECTED
    &&  millis() - tempsDebut < 10000) { // Tentative de connexion pendant 10s
    logger.log(MODULE_WIFI, "Tentative de connexion au réseau : ", String(ssid));

    // Connexion à un réseau to WPA/WPA2
    statut = WiFi.begin(ssid, pwd);

    delay(1000);
  }

  if (statut == WL_CONNECTED) {
    actif = true;
    logger.log(MODULE_WIFI, "Connecté au réseau : ", String(ssid));
  }
  else {    
    actif = false;
    logger.log(MODULE_WIFI, "Erreur de connexion au réseau : ", String(ssid));
  }
}

void Wifi::logStatut()
{
  logger.log(MODULE_WIFI, "Statut", String(WiFi.localIP())
    + " | " + String(WiFi.SSID())
    + " | " + String(WiFi.RSSI()));  
}

String Wifi::typeChiffrement(int type) {
  switch (type) {
    case ENC_TYPE_WEP:
      return String("WEP");
      break;
    case ENC_TYPE_TKIP:
      return String("WPA");
      break;
    case ENC_TYPE_CCMP:
      return String("WPA2");
      break;
    case ENC_TYPE_NONE:
      return String("None");
      break;
    case ENC_TYPE_AUTO:
      return String("Auto");
      break;
    case ENC_TYPE_UNKNOWN:
    default:
      return String("Unknown");
      break;
  }
}
