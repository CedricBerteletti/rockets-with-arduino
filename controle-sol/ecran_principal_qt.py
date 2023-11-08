# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Ecran principal
"""

import logging
import time

import graphiques
import settings


from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QTextCursor
from PyQt5.QtWidgets import QWidget, QHBoxLayout, QVBoxLayout, QGridLayout, QGroupBox, QLineEdit, QTextEdit, QPushButton



class EcranPrincipal(QWidget):
    "Classe représentant l'écran principal"


    def __init__(self, controleur, telemetrie, centrale):
        super().__init__()
        self.actif = True
        self.controleur = controleur
        self.telemetrie = telemetrie
        self.centrale = centrale
        self.graphs_refresh_delay_ms = settings.get_int("graphs.refresh_delay_ms")
        self.init_ui()


    def init_ui(self):
        # Préparation de l'écran
        layout_general = QHBoxLayout()
        self.setLayout(layout_general)
        layout_panneau_gauche = QVBoxLayout()        
        layout_panneau_droit = QVBoxLayout()
        layout_general.addLayout(layout_panneau_gauche)
        layout_general.addLayout(layout_panneau_droit)

        # Panneau de gauche
        self.creer_groupe_visualisation()
        layout_panneau_gauche.addWidget(self.grp_visu)
        self.creer_groupe_commandes()
        layout_panneau_gauche.addWidget(self.grp_command)
        self.creer_groupe_logs()
        layout_panneau_gauche.addWidget(self.grp_logs)
      
        # Panneau de droite
        self.creer_groupe_graphiques()
        layout_panneau_droit.addWidget(self.grp_graphs)
        self.creer_groupe_imu()
        layout_panneau_droit.addWidget(self.grp_imu)

        # Rafraichissement périodique des graphiques et des logs        
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.maj)
        self.timer.start(self.graphs_refresh_delay_ms) # time in milliseconds.


    def creer_groupe_visualisation(self):
        self.grp_visu = QGroupBox("Visualisation")

        # TODO


    def creer_groupe_commandes(self):
        self.grp_command = QGroupBox("Commandes générales")
        layout = QGridLayout()
        self.grp_command.setLayout(layout)

        button = QPushButton("Connecter")
        layout.addWidget(button, 0, 0)
        # TODO

        button = QPushButton("Programmer")
        layout.addWidget(button, 0, 1)
        # TODO

        button = QPushButton("Vider logs")
        layout.addWidget(button, 0, 2)        
        button.clicked.connect(self.vider_logs)

        button = QPushButton("Lancer !")
        layout.addWidget(button, 0, 3)
        # TODO


    def creer_groupe_logs(self):
        self.grp_logs = QGroupBox("Logs généraux")
        layout = QVBoxLayout()
        self.grp_logs.setLayout(layout)

        self.tb_logs = QTextEdit ()
        self.tb_logs.setReadOnly(True)        
        self.tb_logs.setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255); ")
        layout.addWidget(self.tb_logs)
        self.tb_command = QLineEdit ()
        layout.addWidget(self.tb_command)
        self.tb_command.returnPressed.connect(self.nouvelles_commandes)


    def creer_groupe_graphiques(self):
        self.grp_graphs = QGroupBox("Graphiques IMU")
        layout = QVBoxLayout()
        self.grp_graphs.setLayout(layout)

        if settings.get_bool("graphs.debug"):
            self.graphiques = graphiques.BaseGraphiquesIntegres(self.grp_graphs)
        else:
            self.graphiques = graphiques.GraphiquesIntegres(self.grp_graphs)
        layout.addWidget(self.graphiques)


    def creer_groupe_imu(self):
        self.grp_imu = QGroupBox("IMU")
        layout = QVBoxLayout()
        self.grp_imu.setLayout(layout)
       
        layout_command = QGridLayout()
        layout.addLayout(layout_command)

        button = QPushButton("Vider logs")
        layout_command.addWidget(button, 0, 0)
        button.clicked.connect(self.vider_logs_imu)

        button = QPushButton("Calibrer")
        layout_command.addWidget(button, 0, 1)        
        button.clicked.connect(self.calibrer)

        self.tb_imu_logs = QTextEdit ()
        self.tb_imu_logs.setReadOnly(True)
        self.tb_imu_logs.setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255);")
        layout.addWidget(self.tb_imu_logs)


    def maj(self):
        if self.actif:
            logging.debug(self)
            self.maj_loggers()
            self.maj_graph()

    def maj_loggers(self):
        "Mise à jour des loggers"

        # Logs courants
        s = self.telemetrie.logSuivant()
        nouveauLog = False
        while s:
            nouveauLog = True
            self.tb_logs.insertPlainText(s + "\n")
            s = self.telemetrie.logSuivant()
        if nouveauLog:
            self.tb_logs.moveCursor(QTextCursor.End)

        # Logs de la centrale inertielle
        s = self.telemetrie.logImuSuivant()
        nouveauLog = False
        while s:
            nouveauLog = True
            self.tb_imu_logs.insertPlainText(s + "\n")
            self.centrale.ajouter_telemetrie(s)
            s = self.telemetrie.logImuSuivant()
        if nouveauLog:
            self.tb_imu_logs.moveCursor(QTextCursor.End)

        # MAJ graphique sur un échantillon des données
        if nouveauLog:
            self.graphiques.ajouter_telemetrie(self.centrale.data_liste[0].t, self.centrale.courant)

    def maj_graph(self):
        self.graphiques.maj()

    def stop(self):
        logging.debug("Fermeture de la fenêtre principale")
        self.actif = False

    def vider_logs(self):
        self.tb_logs.clear()

    def vider_logs_imu(self):
        self.tb_imu_logs.clear()

    def calibrer(self):
        self.centrale.calibrer()

    def nouvelles_commandes(self):
        commandes = self.tb_command.text().splitlines()  
        self.tb_command.clear()
        self.executer_commandes(commandes)

    def executer_commandes(self, commandes):
        for commande in commandes:
            self.executer_commande(commande)
            # Attente d'envoi de la commande (risque de perte sinon : UDP et non TCP)
            time.sleep(0.2)

    def executer_commande(self, commande):
        self.tb_logs.insertHtml(f"<h1 style=\"color:blue;\">> {commande}</h1>\n")
        self.tb_logs.insertPlainText("\n")
        self.tb_logs.moveCursor(QTextCursor.End)
        if commande[0:13] == "wifi.initUdp ":
            connect, ip, port = commande.split()
            self.controleur.connecter(ip, int(port))
        else:
            pass
            self.controleur.envoyer_commande_brute(commande)
