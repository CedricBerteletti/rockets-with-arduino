# -*- coding: utf-8 -*-

from tkinter import Tk, Text, BOTH, HORIZONTAL, RAISED, W, N, E, S
from tkinter import Frame, Button, Label, LabelFrame, PanedWindow
from tkinter.constants import VERTICAL
from graphique import Graphique


class EcranPrincipal(Frame):

    def __init__(self):
        super().__init__()
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
        visualisation = LabelFrame(panneaux, text="Vue 3D")
        panneauGauche.add(visualisation, height=self.winfo_height()*3/4)
        controle = LabelFrame(panneaux, text="Contrôle")
        panneauGauche.add(controle, height=self.winfo_height()/4)

        

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
        graph4 = Graphique(panneauDroit, text="thetax")
        graph4.grid(column=0, row=3, sticky=(N, S, E, W))
        graph5 = Graphique(panneauDroit, text="thetay")
        graph5.grid(column=0, row=4, sticky=(N, S, E, W))
        graph6 = Graphique(panneauDroit, text="thetaz")
        graph6.grid(column=0, row=5, sticky=(N, S, E, W))
        imuLogs = Text(panneauDroit, bg="#000000", fg="#FFFFFF")
        imuLogs.grid(column=0, row=6, sticky=(N, S, E, W))
