# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Point d'entrée pour le programme du poste de contrôle de la fusée
"""

import logging
import settings

from PyQt5.QtWidgets import QApplication
from PyQt5 import Qt

from centrale_inertielle import CentraleInertielle
from ecran_principal_qt import EcranPrincipal
from connexion import Connexion
from controleur import Controleur
from telemetrie import Telemetrie

import ctypes
import platform

def make_dpi_aware():
    "Permet l'affichage correct des graphiques pyqtgraph sur les moniteurs avec une résolution importante"
    if int(platform.release()) >= 8:
        ctypes.windll.shcore.SetProcessDpiAwareness(True)



def main(args):
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

    # QT - Interface graphique
    make_dpi_aware()
    app = QApplication([])
    ecran_principal = EcranPrincipal(controleur, telemetrie, centrale)
    ecran_principal.showMaximized()
    app.exec()

    # Arrêt de l'application
    logging.debug("On quitte")
    # Fin du thread d'aquisition des données
    telemetrie.stop()
    telemetrie.join()


settings.init()
logging.basicConfig(format="%(asctime)s %(levelname)s - %(filename)s:%(lineno)d - %(message)s", datefmt="%Y-%m-%d %H:%M:%S", level=logging.getLevelName(settings.get("logging.level")))
logging.info("Programme principal")
if __name__ == "__main__":
    from sys import argv
    try:
        main(argv)
    except KeyboardInterrupt:
        pass