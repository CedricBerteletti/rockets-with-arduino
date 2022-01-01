# Fusée avec Arduino embarqué

Quelques programmes pour notre fusée avec Arduino embarqué !

## compte-a-rebours - Compte à rebours

Un compte à rebours simple de 10 secondes, verrouillé par une clé RFID et déclenchant un relais, pour l'allumage électrique d'une micro-fusée.

Bibliothèques nécessaires :
- MFRC522 V1.4 : contrôle de la clé RFID


## logiciel-vol - Logiciel de vol pour l'Arduino embarqué

Bibliothèques nécessaires :
- Arduino_LSM6DS3 : centrale inertielle (CI)
- WiFiNINA : connexion et communication via Wifi

Permet de recevoir via Wifi les commandes :
- connect ip port: permet de connaître le client distant et de lui retourner les mesures et le statut de la fusée
- rocketStatus : état de la fusée
- wifiStatus : état de la connexion Wifi
- toggleLogImuData : active/désactive les logs (via serial, SD-Card et Wifi) des données de la centrale inertielle
- configureStep no_step delai commande : précise la temporisation (en ms) d'une étape donnée et la commande à exécuter au **début** de l'étape
- launch code : passe à l'étape suivante (idem "stage"). Protégé par un code.
- stage code : passe à l'étape suivante (idem "stage"). Protégé par un code.
- stop code : interromp le programme de la fusée et revient à l'étape initiale. Protégé par un code. Attention aux conséquences !
- digitalWrite pin niveau : envoi un signal ou l'arrête sur la sortie numérique désignée.

Remarque sur la carte SD :
- à alimenter en 5V ;
- nom de fichier de 8 caractères maximum (retour à l'époque du DOS !!)


## controle-sol - Centre de Contrôle et de Télémétrie

Application Python permettant de configure le vol de la fusée, de vérifier son statut, de lancer le compte à revours final et d'acquérir et d'afficher les infos de vol.

