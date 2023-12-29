/*
 * --------------------------------------------------------------------------
 * Module gérant les connexions wifi
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#include "Wifi.hpp"

const char Wifi::MODULE_WIFI[] = "WIFI";
const unsigned int Wifi::PORT_LOCAL = 23900;

Wifi::Wifi() {
}

void Wifi::listerReseaux()
{
  char chiffrement[LONGUEUR_MAX_CHAINE_CARACTERES];

  log(MODULE_WIFI, "SEARCHING", "", true);
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    log(MODULE_WIFI, "NO_NETWORK", "", true);
  }
  else {
    itoa(numSsid, strTmp, 10);
    log(MODULE_WIFI, "NETWORKS_FOUND", strTmp, true);

    for (int net = 0; net < numSsid; net++) {
      typeChiffrement(chiffrement, WiFi.encryptionType(net));
      sprintf(strTmp, "%i | %s | Signal : | %i dBm | %s", net, WiFi.SSID(net), WiFi.RSSI(net), chiffrement);
      log(MODULE_WIFI, "NETWORK", strTmp, true);
    }
  }
}



void Wifi::connecter()
{
  flash(PIN_LED_DEBUG);
  connecter(WIFI1_SSID, WIFI1_PASS);
  if (!actif) {
    flash(PIN_LED_DEBUG);
    flash(PIN_LED_DEBUG);
    connecter(WIFI2_SSID, WIFI2_PASS);
  }
}

/* Tente de se connecter à un réseau WiFi
   En cas d'échec, tente de se connecter à l'un des deux réseaux par défaut */
void Wifi::connecterAvecFallback(const char ssid[], const char pwd[])
{
  connecter(ssid, pwd);
  if (!actif) {
    connecter();
  }
  logStatut();
}

void Wifi::connecter(const char ssid[], const char pwd[])
{
  // Déconnexion d'un précédent réseau
  if(strlen(WiFi.SSID()) > 0) {
    log(MODULE_WIFI, "DISCONNECTING", WiFi.SSID(), true);
    WiFi.disconnect();
    delay(5000);
  }

  int statut = WL_IDLE_STATUS;
  long tempsDebut = millis();
  int tempsAttente = 2000; // Attendre l'initialisation de la connexion Wifi pendant 2s
  actif = false;

  while (statut != WL_CONNECTED
      &&  millis() - tempsDebut < 30000) { // Tentative de connexion pendant 30s

    log(MODULE_WIFI, "CONNECTING", ssid, true);

    // Connexion à un réseau WPA/WPA2
    statut = WiFi.begin(ssid, pwd);

    // Attente de la connexion (peut être supérieur à 1s !)
    delay(tempsAttente);
    // Si connexion KO, on attend un peu plus longtemps au prochain essai
    tempsAttente = tempsAttente + 1000;
  }

  if (statut == WL_CONNECTED) {    
    log(MODULE_WIFI, "UDP_BEGIN", ssid, false);
    if(udp.begin(PORT_LOCAL) == 1) {
      actif = true;
      digitalWrite(PIN_LED_DEBUG, 1);
      log(MODULE_WIFI, "CONNECTED", ssid, false);
    }
    else {
      log(MODULE_WIFI, "UDP_NOT_CONNECTED", ssid, true);
    }
  }
  else {
    log(MODULE_WIFI, "ERROR_CONNECTION", ssid, true);
  }
}

void Wifi::logStatut()
{
  char adresse[LONGUEUR_ADRESSE_IP];
  adresseIp(adresse, WiFi.localIP());
  sprintf(strTmp, "%s | %s | %i dBm", adresse, WiFi.SSID(), WiFi.RSSI());
  log(MODULE_WIFI, "STATUS", strTmp, true);  
}

void Wifi::lireUdp(char message[]) {
  if (actif) {
    // Paquet disponible ?
    int taillePaquet = udp.parsePacket();
    if (taillePaquet != 0) {
      currentRemoteIp = udp.remoteIP();
      currentRemotePort = udp.remotePort();

      // Lecture du paquet dans le buffer de réception
      int len = udp.read(message, 255);
      if (len > 0) {
        message[len] = 0;
      }

      logIo("RECEPTION", message);
    }
    else {
      // On indique une chaîne vide
      message[0] = 0;
    }
  }
  else {
    // On indique une chaîne vide
      message[0] = 0;
  }
}

void Wifi::confClientUdp(int n) {
  remoteIps[n] = currentRemoteIp;
  remotePorts[n] = currentRemotePort;
}

void Wifi::ecrireUdp(const char message[]) {
  if (actif && currentRemoteIp) {
    for (unsigned int i = 0; i < NOMBRE_MAX_CLIENTS; i++)
    {
      if (remotePorts[i] != 0) {
        udp.beginPacket(remoteIps[i], remotePorts[i]);
        udp.write(message);
        udp.endPacket();
      }
    }
    logIo("SENDING", message);
  }
}

void Wifi::typeChiffrement(char str[], int type) {
  switch (type) {
    case ENC_TYPE_WEP:
      strcpy(str, "WEP");
      break;
    case ENC_TYPE_TKIP:
      strcpy(str, "WPA");
      break;
    case ENC_TYPE_CCMP:
      strcpy(str, "WPA2");
      break;
    case ENC_TYPE_NONE:
      strcpy(str, "None");
      break;
    case ENC_TYPE_AUTO:
      strcpy(str, "Auto");
      break;
    case ENC_TYPE_UNKNOWN:
    default:
      strcpy(str, "Unknown");
      break;
  }
}

void Wifi::adresseIp(char addresse[], IPAddress ip) {
  uint8_t byte3 = (uint8_t)(ip >> 24);  // 0xde
  uint8_t byte2 = (uint8_t)(ip >> 16);  // 0xad
  uint8_t byte1 = (uint8_t)(ip >> 8);   // 0xbe
  uint8_t byte0 = (uint8_t)ip;
  sprintf(addresse, "%i.%i.%i.%i", byte0, byte1, byte2, byte3);
}

void Wifi::log(const char module[], const char message[], const char details[], bool toWifi) {
  formaterLog(strLog, module, message, details);
  Serial.println(strLog);
  if(actif && toWifi) {
    ecrireUdp(strLog);
  }
}

void Wifi::logIo(const char parameter[], const char message[]) {
  if (loggingIo) {
    char adresse[LONGUEUR_ADRESSE_IP];
    adresseIp(adresse, currentRemoteIp);
    sprintf(strTmp, "%s | %i | %s", adresse, currentRemotePort, message);
    log(MODULE_WIFI, parameter, strTmp, false);
  }
}
