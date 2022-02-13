# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Ecran principal
"""

from tkinter import Tk, StringVar, Text
from tkinter import Button, Entry, Frame, LabelFrame, PanedWindow
from tkinter.constants import VERTICAL, TOP, LEFT, BOTTOM, X, BOTH, VERTICAL, HORIZONTAL, RAISED, DISABLED, NORMAL, W, N, E, S
from graphique import Graphique
import time


class EcranPrincipal(Frame):
    "Classe représentant l'écran principal"


    def __init__(self, controleur, telemetrie, centrale):
        super().__init__()
        self.controleur = controleur
        self.telemetrie = telemetrie
        self.centrale = centrale
        self.init_ui()


    def init_ui(self):
        # Préparation de l'écran
        self.pack(fill=BOTH, expand=True)
        Tk.update(self)
        panneaux = PanedWindow(self, orient=HORIZONTAL, sashrelief = RAISED, sashpad=2)
        panneaux.pack(fill=BOTH, expand=True)

        # Panneau de gauche
        panneauGauche = PanedWindow(panneaux, orient=VERTICAL, sashrelief = RAISED, sashpad=2, width=self.winfo_width()/2)
        panneaux.add(panneauGauche)        

        # 2 panneaux horizontaux de gauche
        visualisation = LabelFrame(panneauGauche, text="Vue")
        panneauGauche.add(visualisation, height=self.winfo_height()*2/3)

        controle = LabelFrame(panneauGauche, text="Contrôle")
        panneauGauche.add(controle, height=self.winfo_height()/3)
        boutons = Frame(controle)
        boutons.pack(side=TOP)
        btnConnect = Button(boutons, text="Connexion")
        btnConnect.pack(padx=5, pady=5, side=LEFT)
        btnEtages = Button(boutons, text="Etages")
        btnEtages.pack(padx=5, pady=5, side=LEFT)
        btnLancer = Button(boutons, text="Lancement !")
        btnLancer.pack(padx=5, pady=5, side=LEFT)
        btnViderLogs = Button(boutons, text="Vider logs", command=self.vider_logs)
        btnViderLogs.pack(padx=5, pady=5, side=LEFT)
        btnViderLogsImu = Button(boutons, text="Vider logs CI", command=self.vider_logs_imu)
        btnViderLogsImu.pack(padx=5, pady=5, side=LEFT)
        btnCalibrerImu = Button(boutons, text="Calibrer CI", command=self.calibrer)
        btnCalibrerImu.pack(padx=5, pady=5, side=LEFT)
        self.logs = Text(controle, height=1, bg="#000000", fg="#FFFFFF")
        self.logs.config(state=DISABLED)
        self.logs.pack(padx=5, pady=5, fill=BOTH, expand=1)
        self.commandeText = StringVar()
        commande = Entry(controle, textvariable=self.commandeText)
        commande.pack(padx=5, pady=5, side=BOTTOM, fill=X)
        commande.bind('<Return>', self.nouvelles_commandes)
      

        # Panneau de droite contenant les informations de trajectoire
        panneauDroit = LabelFrame(panneaux, text="Centrale inertielle", width=self.winfo_width()/2)
        panneaux.add(panneauDroit)
        panneauDroit.columnconfigure(0, weight=1)
        panneauDroit.rowconfigure(0, weight=1)
        panneauDroit.rowconfigure(1, weight=1)
        panneauDroit.rowconfigure(2, weight=1)
        panneauDroit.rowconfigure(3, weight=1)
        panneauDroit.rowconfigure(4, weight=1)
        panneauDroit.rowconfigure(5, weight=1)
        panneauDroit.rowconfigure(6, weight=1)

        self.graph1 = Graphique(panneauDroit, text="x")
        self.graph1.grid(column=0, row=0, sticky=(N, S, E, W))
        self.graph2 = Graphique(panneauDroit, text="y")
        self.graph2.grid(column=0, row=1, sticky=(N, S, E, W))
        self.graph3 = Graphique(panneauDroit, text="z")
        self.graph3.grid(column=0, row=2, sticky=(N, S, E, W))
        self.graph4 = Graphique(panneauDroit, text="alphax")
        self.graph4.grid(column=0, row=3, sticky=(N, S, E, W))
        self.graph5 = Graphique(panneauDroit, text="alphay")
        self.graph5.grid(column=0, row=4, sticky=(N, S, E, W))
        self.graph6 = Graphique(panneauDroit, text="alphaz")
        self.graph6.grid(column=0, row=5, sticky=(N, S, E, W))
        self.imuLogs = Text(panneauDroit, bg="#000000", fg="#FFFFFF")
        self.imuLogs.config(state=DISABLED)
        self.imuLogs.grid(column=0, row=6, sticky=(N, S, E, W))

        self.maj_loggers()


    def maj_loggers(self):
        "Mise à jour des loggers toutes les 100 ms"

        self.logs.config(state=NORMAL)
        s = self.telemetrie.logSuivant()
        nouveauLog = False
        while s:
            nouveauLog = True
            self.logs.insert("end", s + "\n")
            s = self.telemetrie.logSuivant()
        if nouveauLog:
            self.logs.see("end")
        self.logs.config(state=DISABLED)

        self.imuLogs.config(state=NORMAL)
        s = self.telemetrie.logImuSuivant()
        nouveauLog = False
        while s:
            nouveauLog = True
            self.imuLogs.insert("end", s + "\n")
            self.centrale.ajouter_telemetrie(s)
            s = self.telemetrie.logImuSuivant()
        if nouveauLog:
            self.imuLogs.see("end")
        self.imuLogs.config(state=DISABLED)

        # TODO MAJ graphiques
        self.graph1.area.delete(1.0, "end")
        self.graph1.area.insert("end", str(self.centrale.courant.ax) + "\n")
        self.graph1.area.insert("end", str(self.centrale.courant.vx) + "\n")
        self.graph1.area.insert("end", str(self.centrale.courant.x) + "\n")
        self.graph2.area.delete(1.0, "end")
        self.graph2.area.insert("end", str(self.centrale.courant.ay) + "\n")
        self.graph2.area.insert("end", str(self.centrale.courant.vy) + "\n")
        self.graph2.area.insert("end", str(self.centrale.courant.y) + "\n")
        self.graph3.area.delete(1.0, "end")
        self.graph3.area.insert("end", str(self.centrale.courant.az) + "\n")
        self.graph3.area.insert("end", str(self.centrale.courant.vz) + "\n")
        self.graph3.area.insert("end", str(self.centrale.courant.z) + "\n")
        self.graph4.area.delete(1.0, "end")
        self.graph4.area.insert("end", str(self.centrale.courant.valpha) + "\n")
        self.graph4.area.insert("end", str(self.centrale.courant.alpha) + "\n")
        self.graph5.area.delete(1.0, "end")
        self.graph5.area.insert("end", str(self.centrale.courant.vbeta) + "\n")
        self.graph5.area.insert("end", str(self.centrale.courant.beta) + "\n")
        self.graph6.area.delete(1.0, "end")
        self.graph6.area.insert("end", str(self.centrale.courant.vgamma) + "\n")
        self.graph6.area.insert("end", str(self.centrale.courant.gamma) + "\n")

        self.after(100, self.maj_loggers)


    def vider_logs(self):
        self.logs.config(state=NORMAL)
        self.logs.delete(1.0, "end")
        self.logs.config(state=DISABLED)

    def vider_logs_imu(self):
        self.imuLogs.config(state=NORMAL)
        self.imuLogs.delete(1.0, "end")
        self.imuLogs.config(state=DISABLED)
        self.centrale.effacer_donnees()

    def calibrer(self):
        self.centrale.calibrer()

    def nouvelles_commandes(self, event):
        commandes = self.commandeText.get().splitlines()
        self.executer_commandes(commandes)
        self.commandeText.set("")

    def executer_commandes(self, commandes):
        for commande in commandes:
            self.executer_commande(commande)
            # Attente d'envoi de la commande (risque de perte sinon : UDP et non TCP)
            time.sleep(0.2)

    def executer_commande(self, commande):
        if commande[0:13] == "wifi.initUdp ":
            connect, ip, port = commande.split()
            self.controleur.connecter(ip, int(port))
        else:
            self.controleur.envoyer_commande_brute(commande)
