# Fusée avec Arduino embarqué

Quelques programmes pour notre fusée avec Arduino embarqué !

## compte-a-rebours - Compte à rebours

Un compte à rebours simple de 10 secondes, verrouillé par une clé RFID et déclenchant un relais, pour l'allumage électrique d'une micro-fusée.
Basé sur un Arduino Nano.

Bibliothèques nécessaires :
- SPI
- MFRC522 V1.4 : contrôle de la clé RFID


## logiciel-vol - Logiciel de vol pour l'Arduino embarqué

Basé sur un Arduino Nano 33 IoT.

Bibliothèques nécessaires :
- Arduino_LSM6DS3 : centrale inertielle (CI)
- WiFiNINA : connexion et communication via Wifi
- SD : gestion de la carte SD
- Servo : gestion des servomoteurs
- avr/dtostrf : pour convertir les floats en chaîne de caractères

Permet de recevoir via Wifi les commandes :
- Commandes générales
	- *connect ip port*: permet de connaître le client distant et de lui retourner les mesures et le statut de la fusée.
	- *delay duree* : bloque l'exactution de la boucle principale de l'Arduino pendant *duree* ms.
- Loggers et différents états 
	- *rocketStatus* : état de la fusée.
	- *initSdcard* : initialise et lance les logs sur la carte SD.
	- *flushLogs* : force le vidage du cache des logs sur la carte SD.
	- *wifiStatus* : état de la connexion Wifi.
	- *loggerStatus* : indique les différents loggers actifs.
	- *toggleLogImuData* : active/désactive les logs (via serial, SD-Card et Wifi) des données de la centrale inertielle.
	- *toggleLogFlush*: active/désactive les logs de vidage du cache sur la carte SD.
	- *toggleLogRocketStatus* : active/désactive les logs d'état général de la fusée.
- Plan/script de vol de la fusée
	- *configureStep no_step delai commande* : précise la temporisation (en ms) de l'étape *no_step* et la commande à exécuter au **début** de l'étape.
	- *rocketSteps* : renvoie toutes les étapes de vol de la fusée.
	- *pinMode pin mode* : configure la broche n°*pin* (OUTPUT/INPUT). 
- Lancement et actions
	- *digitalWrite pin niveau* : envoie un signal (0 ou 1) ou l'arrête sur la sortie numérique *pin* désignée.
	- *servoPosition servo angle* : positionne sur un *angle* précis (de 0 à 180°) le servo moteur n°*servo*.
	- *launch code* : passe à l'étape suivante (idem "stage"). Protégé par un code.
	- *stage code* : passe à l'étape suivante (idem "stage"). Protégé par un code.
	- *stop code* : interrompt le programme de la fusée et revient à l'étape initiale. Protégé par un code. Attention aux conséquences !
	- *tone pin freq* : joue un son de fréquence *freq* hz sur la sortie *pin*.
	- *toneStop pin* : coupe le son sur la sortie *pin*.

Remarques sur l'Arduino Nano 33 IoT :
- La gestion de l'USB est basé sur un programme chargé par le bootloader. Mais si le programme personnalisé est bogué, il peut empêcher la routine de gestion de l'USB de fonctionner. L'Arduino apparaît alors "briqué". Il faut appuyer rapidement 2 fois sur le boutons reset pour forcer le rechargement du bootloader (et la gestion du port USB).
- **Attention** : I/O en 3,3 V.

Remarques sur la carte SD :
- à alimenter en 5V ;
- nom de fichier de 8 caractères maximum (retour à l'époque du DOS !!)


## controle-sol - Centre de Contrôle et de Télémétrie

Application Python permettant de configurer le vol de la fusée, de vérifier son statut, de lancer le compte à rebours final et d'acquérir et d'afficher les infos de vol.

