/*
* Module gérant les connexions wifi
*/

#include "Wifi.h"

const char Wifi::MODULE_WIFI[] = "WIFI";
const unsigned int Wifi::LOCAL_PORT = 23900;

Wifi::Wifi() {
}

void Wifi::listerReseaux()
{
  log(MODULE_WIFI, "SEARCHING", "", true);
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    log(MODULE_WIFI, "NO_NETWORK", "", true);
  }
  else {
    log(MODULE_WIFI, "NETWORKS_FOUND", String(numSsid), true);

    for (int net = 0; net < numSsid; net++) {
      log(MODULE_WIFI, "NETWORK", String(net)
        + " | " + String(WiFi.SSID(net))
        + " | Signal : " + String(WiFi.RSSI(net)) + " dBm"
        + " | " + typeChiffrement(WiFi.encryptionType(net)),
        true);
    }
  }
}

void Wifi::init(char ssid[], char pwd[])
{
  int statut = WL_IDLE_STATUS;
  long tempsDebut = millis();

  while (statut != WL_CONNECTED
    &&  millis() - tempsDebut < 10000) { // Tentative de connexion pendant 10s
    log(MODULE_WIFI, "CONNECTING", String(ssid), true);

    // Connexion à un réseau to WPA/WPA2
    statut = WiFi.begin(ssid, pwd);

    delay(1000);
  }

  if (statut == WL_CONNECTED) {
    actif = true;
    udp.begin(LOCAL_PORT);
    log(MODULE_WIFI, "CONNECTED", String(ssid), true);
  }
  else {    
    actif = false;
    log(MODULE_WIFI, "ERROR_CONNECTION", String(ssid), true);
  }
}

void Wifi::logStatut()
{
  log(MODULE_WIFI, "STATUS", ipAddress(WiFi.localIP())
    + " | " + String(WiFi.SSID())
    + " | " + String(WiFi.RSSI()),
    true);  
}

String Wifi::lireUdp() {
  if (actif) {
    // Paquet disponible ?
    int taillePaquet = udp.parsePacket();
    if (taillePaquet != 0) {
      remoteIp = udp.remoteIP();
      remotePort = udp.remotePort();

      // Lecture du paquet dans le buffer de réception
      int len = udp.read(bufferReception, 255);
      if (len > 0) {
        bufferReception[len] = 0;
      }

      if (loggingIo) log(MODULE_WIFI, "RECEPTION", ipAddress(udp.remoteIP())
        + " | " + String(udp.remotePort())
        + " | " + String(bufferReception),
        false);
    }
    else {
      // On indique une chaîne vide
      bufferReception[0] = 0;
    }
  }
  return String(bufferReception);
}

void Wifi::ecrireUdp(String msg) {
  if (actif && udp.remoteIP()) {
    msg.toCharArray(bufferEnvoi, 255);
    udp.beginPacket(udp.remoteIP(), LOCAL_PORT);
    udp.write(bufferEnvoi);
    udp.endPacket();

    if (loggingIo) log(MODULE_WIFI, "SENDING", ipAddress(udp.remoteIP())
      + " | " + String(udp.remotePort())
      + " | " + String(bufferEnvoi),
      false);
  }
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

String Wifi::ipAddress(IPAddress ip) {
  uint8_t byte3 = (uint8_t)(ip >> 24);  // 0xde
  uint8_t byte2 = (uint8_t)(ip >> 16);  // 0xad
  uint8_t byte1 = (uint8_t)(ip >> 8);   // 0xbe
  uint8_t byte0 = (uint8_t)ip;
  return String(byte0) + "." + String(byte1) + "." + String(byte2) + "." + String(byte3);
}

void Wifi::log(String module, String message, String details, bool toWifi) {
  //String str = formatLog(module, message, details);
  char str[STRING_MAX_LENGTH];
  formatLog(module, message, details, str)
  Serial.println(str);
  if(actif && toWifi) {
    ecrireUdp(str);
  }
}
