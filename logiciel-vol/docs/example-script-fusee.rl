# Script d'example pour configurer le vol d'une fusée

# Connexion à la fusée et vérification des différents statuts
wifi.initUdp 192.168.0.8 23900
status.wifi
status.logger
status.rocket
rocket.getSteps

# Vérifier le contrôle des entrées-sorties
pin.setMode 2 OUTPUT
pin.setMode 3 OUTPUT
pin.setMode 4 OUTPUT
pin.setMode 5 OUTPUT
pin.setMode 6 OUTPUT
pin.setMode 7 OUTPUT
pin.setMode 8 OUTPUT
pin.setMode 9 OUTPUT
pin.setMode 14 OUTPUT
pin.setMode 15 OUTPUT
pin.setMode 16 OUTPUT
pin.setMode 17 OUTPUT

# Configuration des étapes de vol (stagging)
rocket.configureStep 0 10000 logger.toggleLogFlush
rocket.configureStep 1 5000 pin.digitalWrite 14 1
rocket.configureStep 2 2000 pin.digitalWrite 15 1
rocket.configureStep 3 10000 pin.digitalWrite 16 1
rocket.configureStep 4 0 logger.toggleLogFlush
rocket.configureStep 5 0 pin.tone 9 440
# Boucle (clignotement d'une LED pour la récupération par exemple)
rocket.configureStep 6 500 pin.digitalWrite 17 1
rocket.configureStep 7 500 pin.digitalWrite 17 0
rocket.configureStep 8 0 rocket.goStep 6
rocket.getSteps

# Logs des données de la centrale inertielle
logger.toggleLogImuData

# Lancement
# rocket.launch code

#pin.tone 9 264
#pin.tone 8 297
#pin.tone 7 330
#pin.tone 6 352
#pin.tone 5 396
#pin.tone 4 440
#pin.tone 3 495
#pin.tone 2 528


# servo.setPosition servo angle (de 0 à 180°)
servo.setPosition 1 90

