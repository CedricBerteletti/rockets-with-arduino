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
inline constexpr int LONGUEUR_MAX_CHAINE_CARACTERES = 250;
inline constexpr int LONGUEUR_ADRESSE_IP = 20;

inline constexpr char MODULE_SYSTEME[] = "SYSTEM";

inline constexpr int DELAI_FLUSH = 2000; // (en ms) Flush des logs sur la carte SD toutes les secondes
inline constexpr int DELAI_STATUT = 1000; // (en ms)
inline constexpr int DELAI_IMU = 5; // (en ms)
inline constexpr int DELAI_COMMANDE = 5; // (en ms) Délai entre deux tentatives de lecture d'une nouvelle commande (en ms)
inline constexpr int DELAI_ENVOI_UDP_MIN = 1; // (en ms)
inline constexpr int DELAI_ENVOI_UDP_MAX = 100; // (en ms)

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