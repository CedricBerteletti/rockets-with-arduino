# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Point d'entrée pour le programme du poste de contrôle de la fusée
"""


import ctypes
import logging
import platform
import services.settings as settings

from PyQt6.QtGui import QPalette, QColor, QColorConstants
from PyQt6.QtWidgets import QApplication

from services.centrale_inertielle import CentraleInertielle
from ui.ecran_principal import EcranPrincipal
from services.connexion import Connexion
from services.controleur import Controleur
from services.telemetrie import Telemetrie


def make_dpi_aware():
    "Permet l'affichage correct des graphiques pyqtgraph sur les moniteurs avec une résolution importante"
    # if int(platform.release()) >= 8:    
    #     ctypes.windll.shcore.SetProcessDpiAwareness(True)
    pass


def main(args):
    global ecran_principal

    connexion = Connexion()
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

    # Force le même style sur tous les OS
    app.setStyle(settings.get("theme.main"))
    # Dark mode
    palette = QPalette()
    palette.setColor(QPalette.ColorRole.Window, QColor(53, 53, 53))
    palette.setColor(QPalette.ColorRole.WindowText, QColorConstants.White)
    palette.setColor(QPalette.ColorRole.Base, QColor(25, 25, 25))
    palette.setColor(QPalette.ColorRole.AlternateBase, QColor(53, 53, 53))
    palette.setColor(QPalette.ColorRole.ToolTipBase, QColorConstants.Black)
    palette.setColor(QPalette.ColorRole.ToolTipText, QColorConstants.White)
    palette.setColor(QPalette.ColorRole.Text, QColorConstants.White)
    palette.setColor(QPalette.ColorRole.Button, QColor(53, 53, 53))
    palette.setColor(QPalette.ColorRole.ButtonText, QColorConstants.White)
    palette.setColor(QPalette.ColorRole.BrightText, QColorConstants.Red)
    palette.setColor(QPalette.ColorRole.Link, QColor(42, 130, 218))
    palette.setColor(QPalette.ColorRole.Highlight, QColor(42, 130, 218))
    palette.setColor(QPalette.ColorRole.HighlightedText, QColorConstants.Black)
    app.setPalette(palette)
    # Styles personnalisés
    with open("ui/style.qss","r") as style_file:
        app.setStyleSheet(style_file.read())

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