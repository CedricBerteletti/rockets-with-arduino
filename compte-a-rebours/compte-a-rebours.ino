/*
 * --------------------------------------------------------------------------------------------------------------------
 * Compte à rebours pour l'allumage d'une fusée. Verrouillé par une clé RFID.
 * --------------------------------------------------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN     9     // Configurable, see typical pin layout above
#define SS_PIN      10    // Configurable, see typical pin layout above

#define BOUTON_PIN  4     // Broche à laquelle est relié le bouton de lancement
#define COMMANDE_PIN 8    // Broche de commande du relai de mise à feu

// Broches pour la LED RVB
#define ROUGE_PIN 5
#define VERT_PIN 6
#define BLEU_PIN 7


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Création de l'instance pour la gestion de la clé RFID


MFRC522::MIFARE_Key cleRfid;

void setup() {
  Serial.begin(9600);  // Initialisation du port série pour la communication avec le PC
  while (!Serial);     // Ne rien faire si le port série n'est pas ouvert (pour Arduinos basé ATMEGA32U4)
  SPI.begin();         // Initialisation du bus SPI (auquel est relié la carte de lecture RFID)
  mfrc522.PCD_Init();  // Initialisation de la carte de lecture RFID MFRC522
  
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) {
    cleRfid.keyByte[i] = 0xFF;
  }

  pinMode(BOUTON_PIN, INPUT_PULLUP); 

  pinMode(ROUGE_PIN, OUTPUT);
  pinMode(VERT_PIN, OUTPUT);
  pinMode(BLEU_PIN, OUTPUT);
  digitalWrite(ROUGE_PIN, LOW);
  digitalWrite(VERT_PIN, LOW);
  digitalWrite(BLEU_PIN, LOW);

  pinMode(COMMANDE_PIN, OUTPUT);

  Serial.println(F("Console de lancement en attente ..."));
}

const int STATUT_VERROUILLE = 0;
const int STATUT_DEVERROUILLE = 1;
const int STATUT_COMPTE_A_REBOURS = 2;
const int STATUT_ALLUMAGE = 3;
int statut = STATUT_VERROUILLE;

int intensiteRouge;
int intensiteVert;
int intensiteBleu;

int delai = 1000;


static const char CLE_AUTORISEE[] = "FE 4A 43 73";

// But of course this is a more proper approach
void loop() {
  intensiteRouge = 0;
  intensiteVert = 0;
  intensiteBleu = 0;

  
  if(statut == STATUT_VERROUILLE) {
    // VERIFICATION DE LA CLE RFID
    // Look for new cards, and select one if present
    if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
      delay(50);
      return;
    }  
    // Now a card is selected. The UID and SAK is in mfrc522.uid.
    
    if(verificationCle()) {
      Serial.println("Déverrouillage de la séquence de lancement.");
      statut = STATUT_DEVERROUILLE;

      intensiteVert = 255;
      analogWrite(VERT_PIN, intensiteVert);
      delai = 100;
    }
    else {    
      Serial.println("ALERTE ! Accès non autorisé !");
      
      intensiteRouge = 255;
      analogWrite(ROUGE_PIN, intensiteRouge);
      delay(200);
      analogWrite(ROUGE_PIN, 0);
      delay(200);
      analogWrite(ROUGE_PIN, intensiteRouge);
      delay(200);
      analogWrite(ROUGE_PIN, 0);
      delay(200);
      analogWrite(ROUGE_PIN, intensiteRouge);
      delay(200);
      analogWrite(ROUGE_PIN, 0);
    }
  }
  else if(statut == STATUT_DEVERROUILLE && digitalRead(BOUTON_PIN) == LOW) {
    Serial.println("Séquence finale.");
    statut = STATUT_COMPTE_A_REBOURS;
  }
  else if (statut == STATUT_COMPTE_A_REBOURS) {

    intensiteVert = 40;
    intensiteRouge = 225;
    for(int i = 10; i > 1; i -= 1)
    {
      Serial.println(i);
      analogWrite(ROUGE_PIN, intensiteRouge);
      analogWrite(VERT_PIN, intensiteVert);
      delay(500);
      analogWrite(ROUGE_PIN, 0);
      analogWrite(VERT_PIN, 0);
      delay(500);
    }
    
    intensiteVert = 0;
    intensiteRouge = 255;
    Serial.println(1);
    analogWrite(ROUGE_PIN, intensiteRouge);
    analogWrite(VERT_PIN, intensiteVert);
    delay(800);
    analogWrite(ROUGE_PIN, 0);
    analogWrite(VERT_PIN, 0);
    delay(200);
    statut = STATUT_ALLUMAGE;
  }
  else if (statut == STATUT_ALLUMAGE) {    
    Serial.println("Décollage !");
    // Mise à feu
    intensiteVert = 0;
    intensiteRouge = 255;
    analogWrite(ROUGE_PIN, intensiteRouge);
    analogWrite(VERT_PIN, intensiteVert);
    digitalWrite(COMMANDE_PIN, HIGH);
    
    delay(2000);
    
    digitalWrite(COMMANDE_PIN, LOW);
    analogWrite(ROUGE_PIN, 0);
    analogWrite(VERT_PIN, 0);    
    statut = STATUT_VERROUILLE;  
  }
  
  delay(delai);
}

boolean verificationCle() {
  char cleRfid[20] = "";
  char* pointeurCle = cleRfid;
  char* finBuffer = cleRfid + sizeof(cleRfid);
  for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (pointeurCle + 3 < finBuffer)
      {
          if (i > 0)
          {
              pointeurCle += sprintf(pointeurCle, " ");
          }
          pointeurCle += sprintf(pointeurCle, "%02X", mfrc522.uid.uidByte[i]);
      }
  }
  Serial.print(F("Clé UID:"));
  Serial.print(cleRfid);
  Serial.println();

  if(strcmp(cleRfid, CLE_AUTORISEE) == 0) {
    return true;
  }
  else {    
    return false;
  }
}
