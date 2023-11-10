# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Envoi des commandes à la fusée et réception
"""

from connexion import Connexion


# Dictionnaire de compilation
dict_compilation = {"wifi.connectNetwork": "WC",
                    "wifi.broadcastUdpClient": "WB",
                    "wifi.status": "WS",

                    "system.delay": "0D",

                    "logger.status": "LS",
                    "logger.initSdcard": "LC",
                    "logger.flushToSdcard": "LF",
                    "logger.activateLogImuData": "LI",
                    "logger.deactivateLogImuData": "Li",
                    "logger.activateLogFlush": "LW",
                    "logger.deactivateLogFlush": "Lw",
                    "logger.activateLogRocketStatus": "LR",
                    "logger.deactivateLogRocketStatus": "Lr",
                    
                    "rocket.status": "RS",
                    "rocket.launch": "F0",
                    "rocket.stage": "RE",
                    
                    
                    "flightplan.start": "F0",
                    "flightplan.stop": "FZ",
                    "flightplan.configureStep": "FC",
                    "flightplan.getSteps": "FS",
                    "flightplan.goStep": "FG",
                    
                    "pin.setMode": "PC",
                    "pin.digitalWrite": "PD",
                    "pin.digitalWrite": "PD",
                    "pin.fire": "PD", # TODO
                    "pin.tone": "PT",
                    "pin.toneStop": "Pt",
                    
                    "servo.setPosition": "SP",
                    }



class Controleur():
    "Classe pour gérer les commandes envoyées à la fusée et la réception des informations"

    def __init__(self, connexion):
        self.connexion = connexion

    def connecter(self, ip, port):
        self.connexion.init(ip, port)
        self.envoyer_commande_brute("wifi.broadcastUdpClient " + ip + " " + str(port))

    def envoyer_commande_brute(self, commande_brute):
        commande = self.compiler_commande(commande_brute)
        self.connexion.envoyer(commande)
        return commande

    def compiler_commande(self, commande_brute):
        commande = commande_brute.strip()
        # TODO
        return commande

    def traduire_logs(self, log):
        # TODO
        return log
    
    def traduire_logs_imu(self, log):
        # TODO
        return log
    
        