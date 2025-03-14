# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Écran principal
"""


import logging
import time

from PyQt6.QtCore import Qt, QTimer
from PyQt6.QtGui import QTextCursor
from PyQt6.QtWidgets import QFrame, QHBoxLayout, QVBoxLayout, QGridLayout, QFileDialog, \
QGroupBox, QLineEdit, QTextEdit, QPushButton, QSplitter, QCheckBox, QLabel, QDialog, QDialogButtonBox

import ui.graphiques as graphiques
from ui.ui_utils import set_default_layout_params
from ui.visualisation_fusee import VisualisationFusee
import services.settings as settings


class EcranPrincipal(QFrame):
    "Classe représentant l'écran principal"


    def __init__(self, controleur, telemetrie, centrale):
        super().__init__()
        self.actif = True
        self.controleur = controleur
        self.telemetrie = telemetrie
        self.centrale = centrale
        self.graphs_refresh_delay_ms = settings.get_int("graphs.refresh_delay_ms")
        self.ip = settings.get("telemetry.ip")
        self.port = settings.get("telemetry.port")
        self.password = ""
        self.init_ui()

    
    ###  CRÉATION DE L'INTERFACE GRAPHIQUE  ###

    def init_ui(self):
        # Préparation de l'écran
        splitter_general = QSplitter(Qt.Orientation.Horizontal)
        self.setLayout(QHBoxLayout())
        set_default_layout_params(self.layout())
        self.layout().addWidget(splitter_general)

        # Panneau de gauche
        panneau_gauche = QFrame()
        panneau_gauche.setLayout(QVBoxLayout())
        set_default_layout_params(panneau_gauche.layout())
        splitter_general.addWidget(panneau_gauche)
        splitter_panneau_gauche = QSplitter(Qt.Orientation.Vertical)
        panneau_gauche.layout().addWidget(splitter_panneau_gauche)

        frame = QFrame()
        splitter_panneau_gauche.addWidget(frame)
        frame.setLayout(QVBoxLayout())
        set_default_layout_params(frame.layout())
        self.creer_groupe_visualisation()
        frame.layout().addWidget(self.grp_visu)

        frame = QFrame()
        splitter_panneau_gauche.addWidget(frame)
        frame.setLayout(QVBoxLayout())
        set_default_layout_params(frame.layout())
        self.creer_groupe_commandes()
        frame.layout().addWidget(self.grp_command)
        self.creer_groupe_logs()
        frame.layout().addWidget(self.grp_logs)
      
        # Panneau de droite
        panneau_droit = QFrame()
        panneau_droit.setLayout(QVBoxLayout())
        set_default_layout_params(panneau_droit.layout())
        splitter_general.addWidget(panneau_droit)        
        splitter_panneau_droite = QSplitter(Qt.Orientation.Vertical)
        panneau_droit.layout().addWidget(splitter_panneau_droite)

        self.creer_groupe_graphiques()
        splitter_panneau_droite.addWidget(self.grp_graphs)
        self.creer_groupe_imu()
        splitter_panneau_droite.addWidget(self.grp_imu)

        # Rafraichissement périodique des graphiques et des logs        
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.maj)
        self.timer.start(self.graphs_refresh_delay_ms) # time in milliseconds.


    def creer_groupe_visualisation(self):
        self.grp_visu = QGroupBox("Visualisation")
        self.grp_visu.setLayout(QVBoxLayout())
        set_default_layout_params(self.grp_visu.layout())
        self.visualisation = VisualisationFusee()
        self.grp_visu.layout().addWidget(self.visualisation)


    def creer_groupe_commandes(self):
        self.grp_command = QGroupBox("Commandes générales")
        self.grp_command.setLayout(QGridLayout())
        set_default_layout_params(self.grp_command.layout())

        button = QPushButton("Connecter")
        self.grp_command.layout().addWidget(button, 0, 0)
        button.clicked.connect(self.demander_ip_port)

        button = QPushButton("Charger")
        self.grp_command.layout().addWidget(button, 0, 1)
        button.clicked.connect(self.charger_programme)

        button = QPushButton("Vider logs")
        self.grp_command.layout().addWidget(button, 0, 2)        
        button.clicked.connect(self.vider_logs)

        button = QPushButton("Lancer !")
        self.grp_command.layout().addWidget(button, 0, 3)
        button.clicked.connect(self.confirmer_lancement)

        button = QPushButton("Stop !")
        self.grp_command.layout().addWidget(button, 0, 4)
        button.clicked.connect(self.confirmer_stop)


    def creer_groupe_logs(self):
        self.grp_logs = QGroupBox("Logs généraux")
        self.grp_logs.setLayout(QVBoxLayout())
        set_default_layout_params(self.grp_logs.layout())

        self.tb_logs = QTextEdit ()
        self.tb_logs.setReadOnly(True)        
        self.tb_logs.setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255); ")
        self.grp_logs.layout().addWidget(self.tb_logs)
        self.tb_command = QLineEdit ()
        self.grp_logs.layout().addWidget(self.tb_command)
        self.tb_command.returnPressed.connect(self.nouvelles_commandes)


    def creer_groupe_graphiques(self):
        self.grp_graphs = QGroupBox("Graphiques IMU")
        self.grp_graphs.setLayout(QVBoxLayout())
        set_default_layout_params(self.grp_graphs.layout())

        if settings.get_bool("graphs.debug"):
            self.graphiques = graphiques.BaseGraphiquesIntegres(self.grp_graphs)
        else:
            self.graphiques = graphiques.GraphiquesIntegres(self.grp_graphs)
        self.grp_graphs.layout().addWidget(self.graphiques)

    

    ###  FONCTIONS DE L'INTERFACE GRAPHIQUE  ###

    def demander_ip_port(self):
        dialog = QDialog(self)
        dialog.setWindowTitle("Connexion")
        dialog.setLayout(QVBoxLayout())

        ip_label = QLabel("Adresse IP:")
        ip_input = QLineEdit()
        ip_input.setText(self.ip)
        port_label = QLabel("Port:")
        port_input = QLineEdit()
        port_input.setText(self.port)

        dialog.layout().addWidget(ip_label)
        dialog.layout().addWidget(ip_input)
        dialog.layout().addWidget(port_label)
        dialog.layout().addWidget(port_input)

        button_box = QDialogButtonBox(QDialogButtonBox.StandardButton.Ok | QDialogButtonBox.StandardButton.Cancel)
        button_box.accepted.connect(dialog.accept)
        button_box.rejected.connect(dialog.reject)
        dialog.layout().addWidget(button_box)

        if dialog.exec():
            self.ip = ip_input.text()
            self.port = port_input.text()
            self.executer_commande(f"wifi.broadcastUdpClient 0 {self.ip} {self.port}")

    def creer_groupe_imu(self):
        self.grp_imu = QGroupBox("IMU")
        self.grp_imu.setLayout(QVBoxLayout())
        set_default_layout_params(self.grp_imu.layout())
       
        layout_command = QGridLayout()
        set_default_layout_params(layout_command)
        self.grp_imu.layout().addLayout(layout_command)

        button = QPushButton("Vider logs")
        layout_command.addWidget(button, 0, 0)
        button.clicked.connect(self.vider_logs_imu)

        button = QPushButton("Vider et effacer")
        layout_command.addWidget(button, 0, 1)
        button.clicked.connect(self.imu_vider_et_effacer)

        button = QPushButton("Calibrer")
        layout_command.addWidget(button, 0, 2)        
        button.clicked.connect(self.calibrer)

        self.cb_imu_logs_defil = QCheckBox("Défilement auto")
        self.cb_imu_logs_defil.setChecked(True)
        layout_command.addWidget(self.cb_imu_logs_defil, 0, 3)

        self.tb_imu_logs = QTextEdit ()
        self.tb_imu_logs.setReadOnly(True)
        self.tb_imu_logs.setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255);")
        self.grp_imu.layout().addWidget(self.tb_imu_logs)

    def charger_programme(self):
        fileName = QFileDialog.getOpenFileName(self, "Charger programme de la fusée", "",
                                               "Fichiers rfp (*.rfp)")
        with open(fileName[0]) as f:
            lines = f.read().splitlines()
            self.executer_commandes(lines)

    def confirmer_lancement(self):
        dialog = QDialog(self)
        dialog.setWindowTitle("Lancement !")
        dialog.setLayout(QVBoxLayout())

        password_label = QLabel("Mot de passe !")
        password_input = QLineEdit()
        password_input.setText(self.password)

        dialog.layout().addWidget(password_label)
        dialog.layout().addWidget(password_input)

        button_box = QDialogButtonBox(QDialogButtonBox.StandardButton.Ok | QDialogButtonBox.StandardButton.Cancel)
        button_box.accepted.connect(dialog.accept)
        button_box.rejected.connect(dialog.reject)
        dialog.layout().addWidget(button_box)

        if dialog.exec():
            self.password = password_input.text()
            self.executer_commande(f"rocket.launch {self.password}")

    def confirmer_stop(self):
        if self.password:
            self.executer_commande(f"flightplan.stop {self.password}")
        else:
            dialog = QDialog(self)
            dialog.setWindowTitle("Stop !")
            dialog.setLayout(QVBoxLayout())

            password_label = QLabel("Mot de passe :")
            password_input = QLineEdit()
            password_input.setText(self.password)

            dialog.layout().addWidget(password_label)
            dialog.layout().addWidget(password_input)

            button_box = QDialogButtonBox(QDialogButtonBox.StandardButton.Ok | QDialogButtonBox.StandardButton.Cancel)
            button_box.accepted.connect(dialog.accept)
            button_box.rejected.connect(dialog.reject)
            dialog.layout().addWidget(button_box)

            if dialog.exec():
                self.password = password_input.text()
                self.executer_commande(f"flightplan.stop {self.password}")

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
            s = self.controleur.traduire_logs(s)
            self.tb_logs.insertPlainText(s + "\n")
            s = self.telemetrie.logSuivant()
        if nouveauLog:
            self.tb_logs.moveCursor(QTextCursor.MoveOperation.End)

        # Logs de la centrale inertielle
        s = self.telemetrie.logImuSuivant()
        nouveauLog = False
        while s:
            nouveauLog = True
            self.centrale.ajouter_telemetrie(s)
            s = self.controleur.traduire_logs_imu(s)
            self.tb_imu_logs.insertPlainText(s + "\n")
            s = self.telemetrie.logImuSuivant()
        if nouveauLog and self.cb_imu_logs_defil.isChecked():
            self.tb_imu_logs.moveCursor(QTextCursor.MoveOperation.End)

        # MAJ graphique sur un échantillon des données
        if nouveauLog:
            self.graphiques.ajouter_telemetrie(self.centrale.data_liste[0].t, self.centrale.courant)

    def maj_graph(self):
        self.graphiques.maj()
        self.visualisation.maj_orientation(self.centrale.courant.alpha, self.centrale.courant.beta, self.centrale.courant.gamma)

    def stop(self):
        logging.debug("Fermeture de la fenêtre principale")
        self.actif = False

    def vider_logs(self):
        self.tb_logs.clear()

    def vider_logs_imu(self):
        self.tb_imu_logs.clear()

    def imu_vider_et_effacer(self):
        self.vider_logs_imu()
        self.graphiques.effacer()
        self.centrale.effacer_donnees()

    def calibrer(self):
        self.centrale.calibrer()
        self.imu_vider_et_effacer()
        self.maj_graph()

    def nouvelles_commandes(self):
        commandes = self.tb_command.text().splitlines()
        self.tb_command.clear()
        self.executer_commandes(commandes)
        self.visualisation.raz()

    def executer_commandes(self, commandes):
        for commande in commandes:
            self.executer_commande(commande)
            # Attente d'envoi de la commande (risque de perte sinon : UDP et non TCP)
            time.sleep(0.05)

    def executer_commande(self, commande):
        self.tb_logs.insertHtml(f"<h1 style=\"color:blue;\">> {commande}</h1>\n")
        self.tb_logs.insertPlainText("\n")
        
        if commande[0:24] == "wifi.broadcastUdpClient ":
            connect, pc_id, rocket_ip, rocket_port = commande.split()
            commande_envoyee = self.controleur.connecter(pc_id, rocket_ip, int(rocket_port))
        else:
            commande_envoyee = self.controleur.envoyer_commande_brute(commande)
        self.tb_logs.insertPlainText(f"Commande envoyée : {commande_envoyee}\n")

        self.tb_logs.moveCursor(QTextCursor.MoveOperation.End)
