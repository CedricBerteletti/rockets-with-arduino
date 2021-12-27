/*
 * Module principal du programme de la fusée
 */

static const String MODULE_SYSTEM = "SYSTEM";

/* Variable globales */
unsigned long time = 0;  //Durée en millisecondes depuis le démarrage de l'Arduino
float accX = 0.0f;
float accY = 0.0f;
float accZ = 0.0f;
float alphaX = 0.0f;
float alphaY = 0.0f;
float alphaZ = 0.0f;

void setup() {
  log(MODULE_SYSTEM, "Initialisation générale", "");
  initSerial();
  initImu();

  delay(2000);
}

void loop() {
  lireImu();
}


void initSerial() {
  Serial.begin(250000);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  log(MODULE_SYSTEM, "Liaison série initialisée", "");
}
