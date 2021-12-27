/*
* Logger
*/

// Fonction d'écriture des logs (vers le port série)
void log(String module, String message, String details) {
  const String SEPARATEUR = ", ";
  const String GUILLEMETS = "\"";

  time = millis();

  String log = String(String(time) + SEPARATEUR
    + module + SEPARATEUR
    + GUILLEMETS + message + GUILLEMETS + SEPARATEUR
    + GUILLEMETS + details + GUILLEMETS);

  Serial.println(log);  
}
