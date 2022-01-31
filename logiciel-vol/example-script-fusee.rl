# Script d'example pour configurer le vol d'une fusée

# Connexion à la fusée et vérification des différents statuts
connect 192.168.0.8 23900
wifiStatus
loggerStatus
rocketStatus
rocketSteps

# Vérifier le contrôle des entrées-sorties
pinMode 2 OUTPUT
pinMode 3 OUTPUT
pinMode 4 OUTPUT
pinMode 5 OUTPUT
pinMode 6 OUTPUT
pinMode 7 OUTPUT
pinMode 8 OUTPUT
pinMode 9 OUTPUT
pinMode 14 OUTPUT
pinMode 15 OUTPUT
pinMode 16 OUTPUT
pinMode 17 OUTPUT

# Configuration des étapes de vol (stagging)
configureStep 0 10000 toggleLogFlush
configureStep 1 5000 digitalWrite 14 1
configureStep 2 2000 digitalWrite 15 1
configureStep 3 10000 digitalWrite 16 1
configureStep 4 10000 digitalWrite 17 1
configureStep 5 1 toggleLogFlush
configureStep 6 -1 tone 9 440
rocketSteps

# Logs des données de la centrale inertielle
toggleLogImuData

# Lancement
# launch code

#tone 9 264
#tone 8 297
#tone 7 330
#tone 6 352
#tone 5 396
#tone 4 440
#tone 3 495
#tone 2 528

