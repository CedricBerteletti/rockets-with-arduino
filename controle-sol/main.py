# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Point d'entrée pour le programme du poste de contrôle de la fusée
"""

import logging

from tkinter import Tk
from centrale_inertielle import CentraleInertielle
from ecran_principal import EcranPrincipal
from connexion import Connexion
from controleur import Controleur
from telemetrie import Telemetrie
from time import sleep

# Constantes
LARGEUR_ECRAN = 1280
HAUTEUR_ECRAN = 1024
ARDUINO_IP = "192.168.0.8"
ARDUINO_PORT = 23900



def main(args):
    connexion = Connexion()
    connexion.init(ARDUINO_IP, ARDUINO_PORT)
    controleur = Controleur(connexion)
    telemetrie = Telemetrie(connexion)
    telemetrie.start()
    centrale = CentraleInertielle()
    
    fenetre = Tk("")
    fenetre.title("Centre de Contrôle et de Télémétrie")
    fenetre.geometry(str(LARGEUR_ECRAN) + "x" + str(HAUTEUR_ECRAN))
    principal = EcranPrincipal(controleur, telemetrie, centrale)

    # Boucle principale de l'application
    fenetre.mainloop()

    # Arrêt de l'application
    # Fin du thread d'aquisition des données
    telemetrie.actif = False
    sleep(1)
    connexion.fermer()


logging.basicConfig(format="%(asctime)s %(levelname)s - %(filename)s:%(lineno)d - %(message)s", datefmt="%Y-%m-%d %H:%M:%S", level=logging.DEBUG)
logging.info("Programme principal")
if __name__ == "__main__":
    from sys import argv
    try:
        main(argv)
    except KeyboardInterrupt:
        pass