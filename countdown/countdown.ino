/*
 * --------------------------------------------------------------------------------------------------------------------
 * Simple countdown for a rocket ignition, locked with a RFID key.
 * --------------------------------------------------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN     9     // Configurable, see typical pin layout above
#define SS_PIN      10    // Configurable, see typical pin layout above

#define BUTTON_PIN  4
#define COMMAND_PIN 8

#define BLUE 7
#define GREEN 6
#define RED 5

//#define delayTime 1000


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

/* Set your new UID here! */
#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}

MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);  // Initialize serial communications with the PC
  while (!Serial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  pinMode(COMMAND_PIN, OUTPUT);

  Serial.println(F("Console de lancement en attente ..."));
}

const int STATUS_LOCKED = 0;
const int STATUS_UNLOCKED = 1;
const int STATUS_COUNTDOWN = 2;
const int STATUS_IGNITION = 3;
int status = STATUS_LOCKED;

int redValue;
int greenValue;
int blueValue;

int delayTime = 1000;


static const char AUTHORIZED_KEY[] = "FE 4A 43 73";

// But of course this is a more proper approach
void loop() {
  greenValue = 0;
  redValue = 0;
  blueValue = 0;

  
  if(status == STATUS_LOCKED) {
    // VERIFICATION DE LA CLE RFID
    // Look for new cards, and select one if present
    if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
      delay(50);
      return;
    }  
    // Now a card is selected. The UID and SAK is in mfrc522.uid.
    
    if(checkKey()) {
      Serial.println("Déverrouillage de la séquence de lancement.");
      status = STATUS_UNLOCKED;

      greenValue = 255;
      analogWrite(GREEN, greenValue);
      delayTime = 100;
    }
    else {    
      Serial.println("ALERTE ! Accès non autorisé !");
      
      redValue = 255;
      analogWrite(RED, redValue);
      delay(200);
      analogWrite(RED, 0);
      delay(200);
      analogWrite(RED, redValue);
      delay(200);
      analogWrite(RED, 0);
      delay(200);
      analogWrite(RED, redValue);
      delay(200);
      analogWrite(RED, 0);
    }
  }
  else if(status == STATUS_UNLOCKED && digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Séquence finale.");
    status = STATUS_COUNTDOWN;
  }
  else if (status == STATUS_COUNTDOWN) {

    greenValue = 40;
    redValue = 225;
    for(int i = 10; i > 1; i -= 1)
    {
      Serial.println(i);
      analogWrite(RED, redValue);
      analogWrite(GREEN, greenValue);
      delay(500);
      analogWrite(RED, 0);
      analogWrite(GREEN, 0);
      delay(500);
    }
    
    greenValue = 0;
    redValue = 255;
    Serial.println(1);
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    delay(800);
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    delay(200);
    status = STATUS_IGNITION;
  }
  else if (status == STATUS_IGNITION) {    
    Serial.println("Décollage !");
    // TODO mise à feu
    greenValue = 0;
    redValue = 255;
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    digitalWrite(COMMAND_PIN, HIGH);
    
    delay(2000);
    
    digitalWrite(COMMAND_PIN, LOW);
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);    
    status = STATUS_LOCKED;  
  }

  
  
  delay(delayTime);
}

boolean checkKey() {
  // Dump UID
  char key[20] = "";
  char* keyPointer = key;
  char* endOfBuf = key + sizeof(key);
  for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (keyPointer + 3 < endOfBuf)
      {
          if (i > 0)
          {
              keyPointer += sprintf(keyPointer, " ");
          }
          keyPointer += sprintf(keyPointer, "%02X", mfrc522.uid.uidByte[i]);
      }
  }
  //buf2 += sprintf(buf2, "\n");
  //strcat(dest, src);
  Serial.print(F("Clé UID:"));
  //for (byte i = 0; i < mfrc522.uid.size; i++) {
  //  strcat(key, mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
  //  strcat(key, mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    
  //  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
  //  Serial.print(mfrc522.uid.uidByte[i], HEX);
  //} 
  Serial.print(key);
  Serial.println();

  if(strcmp(key, AUTHORIZED_KEY) == 0) {
    return true;
  }
  else {    
    return false;
  }
}
