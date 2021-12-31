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
- connect : permet de connaître le client distant et de lui retourner les mesures et le statut de la fusée
- toggleLogImuData : active/désactive les logs (via serial, SD-Card et Wifi) des données de la centrale inertielle
- rocketStatus : état de la fusée
- wifiStatus : état de la connexion Wifi


## controle-sol - Centre de Contrôle et de Télémétrie

Application Python permettant de configure le vol de la fusée, de vérifier son statut, de lancer le compte à revours final et d'acquérir et d'afficher les infos de vol.

