/*
 * --------------------------------------------------------------------------
 * Constantes générales
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#ifndef constantes_h
#define constantes_h

// Constantes générales

inline constexpr char CHAINE_VIDE[] = "";
inline constexpr char SEPARATEUR[] = ", ";
inline constexpr char GUILLEMETS[] = "\"";
inline constexpr int LONGUEUR_NOMBRE = 10;
inline constexpr int LONGUEUR_MAX_COMMANDE = 50;
inline constexpr int LONGUEUR_MAX_CHAINE_CARACTERES = 256;
inline constexpr int LONGUEUR_ADRESSE_IP = 20;

inline constexpr char MODULE_SYSTEME[] = "SYSTEM";

inline constexpr long DELAI_FLUSH = 2000; // (en ms) Flush des logs sur la carte SD toutes les secondes
inline constexpr long DELAI_STATUT = 1000; // (en ms)
inline constexpr long DELAI_IMU = 10; // (en ms)
inline constexpr long DELAI_COMMANDE = 50; // (en ms) Tentative de lecture d'une nouvelle commande toutes les 50 ms

inline constexpr bool LOGGING_WIFI_IO = false;

inline constexpr int PIN_LED_DEBUG = 8;
inline constexpr int TAILLE_BUFFER_DONNEES_INERTIELLES = 50;

inline constexpr int NB_SERVOS = 4;
inline constexpr int SERVO0_PIN = 7;
inline constexpr int SERVO1_PIN = 6;
inline constexpr int SERVO2_PIN = 20;
inline constexpr int SERVO3_PIN = 21;

// Nombre d'étapes/instructions du programme de vol
inline constexpr int NB_ETAPES = 50;
inline constexpr int ETAPE_CONFIGURATION = -1;
inline constexpr int ETAPE_DECOMPTE_FINAL = 0;

#endif