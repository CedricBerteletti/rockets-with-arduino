# Script d'example pour configurer le vol d'une fusée

# Connexion à la fusée et vérification des différents statuts
wifi.broadcastUdpClient 0 192.168.0.3 23900
wifi.status
logger.status
rocket.status
flightplan.getSteps

# Vérifier le contrôle des entrées-sorties
pin.setMode 2 OUTPUT
pin.setMode 3 OUTPUT
pin.setMode 4 OUTPUT
pin.setMode 5 OUTPUT
pin.setMode 6 OUTPUT
pin.setMode 7 OUTPUT

pin.setMode 8 OUTPUT
pin.digitalWrite 8 1
pin.setMode 9 OUTPUT
pin.digitalWrite 9 1

pin.setMode 14 OUTPUT
pin.setMode 15 OUTPUT
pin.setMode 16 OUTPUT
pin.setMode 17 OUTPUT


# Configuration des étapes de vol (stagging)
flightplan.configureStep 0 10000 logger.activateLogFlush
flightplan.configureStep 1 5000 pin.digitalWrite 14 1
flightplan.configureStep 2 2000 pin.digitalWrite 15 1
flightplan.configureStep 3 10000 pin.digitalWrite 16 1
flightplan.configureStep 4 0 logger.deactivateLogFlush
flightplan.configureStep 5 0 pin.tone 9 440
# Boucle (clignotement d'une LED pour la récupération par exemple)
flightplan.configureStep 6 500 pin.digitalWrite 17 1
flightplan.configureStep 7 500 pin.digitalWrite 17 0
flightplan.configureStep 8 0 flightplan.goStep 6
flightplan.getSteps

# Logs des données de la centrale inertielle
logger.activateLogImuData
imu.getBuffer

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


# servo.setAngle servo angle (de 0 à 180°)
servo.setAngle 1 90

# Tests servo
flightplan.configureStep 0 500 servo.setAngle 0 90
flightplan.configureStep 1 500 servo.setAngle 1 90
flightplan.configureStep 2 500 servo.setAngle 0 110
flightplan.configureStep 3 500 servo.setAngle 1 110
flightplan.configureStep 4 500 servo.setAngle 1 90
flightplan.configureStep 5 500 servo.setAngle 1 70
flightplan.configureStep 6 500 servo.setAngle 0 90
flightplan.configureStep 7 500 servo.setAngle 0 70
flightplan.configureStep 8 0 flightplan.goStep 0



# Test boucle
flightplan.configureStep 0 000 logger.activateLogImuData
flightplan.configureStep 1 500 pin.digitalWrite 17 1
flightplan.configureStep 2 500 pin.digitalWrite 17 0
flightplan.configureStep 3 0 flightplan.goStep 1
