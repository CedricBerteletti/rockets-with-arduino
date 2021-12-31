# -*- coding: utf-8 -*-

from tkinter import Tk, StringVar, Text
from tkinter import Button, Entry, Frame, LabelFrame, PanedWindow
from tkinter.constants import VERTICAL, TOP, LEFT, BOTTOM, X, BOTH, VERTICAL, HORIZONTAL, RAISED, DISABLED, NORMAL, W, N, E, S
from graphique import Graphique
import time


class EcranPrincipal(Frame):
    "Classe représentant l'écran principal"


    def __init__(self, controleur, telemetrie):
        super().__init__()
        self.controleur = controleur
        self.telemetrie = telemetrie
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
        btnConnect.pack(padx=5, pady=5,side=LEFT)
        btnEtages = Button(boutons, text="Etages")
        btnEtages.pack(padx=5, pady=5,side=LEFT)        
        self.logs = Text(controle, height=1, bg="#000000", fg="#FFFFFF")
        self.logs.config(state=DISABLED)
        self.logs.pack(padx=5, pady=5, fill=BOTH, expand=1)
        self.commandeText = StringVar()
        commande = Entry(controle, textvariable=self.commandeText)
        commande.pack(padx=5, pady=5, side=BOTTOM, fill=X)
        commande.bind('<Return>', self.nouvelleCommande)
      

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

        graph1 = Graphique(panneauDroit, text="x")
        graph1.grid(column=0, row=0, sticky=(N, S, E, W))
        graph2 = Graphique(panneauDroit, text="y")
        graph2.grid(column=0, row=1, sticky=(N, S, E, W))
        graph3 = Graphique(panneauDroit, text="z")
        graph3.grid(column=0, row=2, sticky=(N, S, E, W))
        graph4 = Graphique(panneauDroit, text="alphax")
        graph4.grid(column=0, row=3, sticky=(N, S, E, W))
        graph5 = Graphique(panneauDroit, text="alphay")
        graph5.grid(column=0, row=4, sticky=(N, S, E, W))
        graph6 = Graphique(panneauDroit, text="alphaz")
        graph6.grid(column=0, row=5, sticky=(N, S, E, W))
        self.imuLogs = Text(panneauDroit, bg="#000000", fg="#FFFFFF")
        self.imuLogs.config(state=DISABLED)
        self.imuLogs.grid(column=0, row=6, sticky=(N, S, E, W))

        self.maj_loggers()


    def maj_loggers(self):
        "Mise à jour des loggers toutes les 100 ms"

        self.logs.config(state=NORMAL)
        str = self.telemetrie.logSuivant()
        nouveauLog = False
        while str:
            nouveauLog = True
            self.logs.insert("end", str + "\n")
            str = self.telemetrie.logSuivant()
        if nouveauLog:
            self.logs.see("end")
        self.logs.config(state=DISABLED)

        self.imuLogs.config(state=NORMAL)
        str = self.telemetrie.logImuSuivant()
        nouveauLog = False
        while str:
            nouveauLog = True
            self.imuLogs.insert("end", str + "\n")
            str = self.telemetrie.logImuSuivant()
        if nouveauLog:
            self.imuLogs.see("end")
        self.imuLogs.config(state=DISABLED)

        self.after(100, self.maj_loggers)


    def nouvelleCommande(self, event):
        # self.logs.config(state=NORMAL)
        # self.logs.insert("end", self.commandeText.get()+"\n")
        # self.logs.see("end")
        # self.logs.config(state=DISABLED)
        # self.commandeText.set("")
        commande = self.commandeText.get()
        if commande[0:8] == "connect ":
            connect, ip, port = commande.split()
            self.controleur.connecter(ip, int(port))
        else:
            self.controleur.envoyerCommandeBrute(self.commandeText.get())
        self.commandeText.set("")
