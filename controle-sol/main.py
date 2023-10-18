# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Point d'entrée pour le programme du poste de contrôle de la fusée
"""

import logging
import settings

from tkinter import Tk
from tkinter import messagebox

from centrale_inertielle import CentraleInertielle
from ecran_principal import EcranPrincipal
from connexion import Connexion
from controleur import Controleur
from telemetrie import Telemetrie
from time import sleep



def on_closing():
    global fenetre
    global ecran_principal
    if messagebox.askokcancel("Quitter", "Voulez-vous vraiment quitter ?"):
        ecran_principal.stop()
        fenetre.quit()
        #Tk.tk.quit()
        fenetre.destroy()


def main(args):
    global fenetre
    global ecran_principal

    connexion = Connexion()
    #connexion.init(settings.get("telemetry.arduino.ip"), settings.get("telemetry.arduino.port"))
    controleur = Controleur(connexion)
    telemetrie = Telemetrie(connexion)
    if not settings.get_bool("telemetry.debug"):
        telemetrie.start()
    else:
        telemetrie.startDebug()
    centrale = CentraleInertielle()    
 
    fenetre = Tk("")
    fenetre.title("Centre de Contrôle et de Télémétrie")
    fenetre.geometry(str(settings.get("screen.width")) + "x" + str(settings.get("screen.height")))
    ecran_principal = EcranPrincipal(controleur, telemetrie, centrale)

    # Boucle principale de l'application
    fenetre.protocol("WM_DELETE_WINDOW", on_closing)
    fenetre.mainloop()

    # Arrêt de l'application
    logging.debug("On quitte")
    # Fin du thread d'aquisition des données
    telemetrie.stop()
    telemetrie.join()
    connexion.fermer()
    sleep(1)


settings.init()
logging.basicConfig(format="%(asctime)s %(levelname)s - %(filename)s:%(lineno)d - %(message)s", datefmt="%Y-%m-%d %H:%M:%S", level=logging.getLevelName(settings.get("logging.level")))
logging.info("Programme principal")
if __name__ == "__main__":
    from sys import argv
    try:
        main(argv)
    except KeyboardInterrupt:
        pass