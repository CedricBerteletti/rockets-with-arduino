# Script d'example pour configurer le vol d'une fusée

# Connexion à la fusée et vérification des différents statuts
connect 192.168.0.50 23900
wifiStatus
loggerStatus
rocketStatus
delay 2000

# Vérifier le contrôle des entrées-sorties
pinMode 13 OUTPUT
delay 2000
digitalWrite 13 1

# Configuration des étapes de vol (stagging)
configureStep 0 10000 digitalWrite 13 1

# Logs des données de la centrale inertielle
toggleLogImuData

# Lancement
# launch code

