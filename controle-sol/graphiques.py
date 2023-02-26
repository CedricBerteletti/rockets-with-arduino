# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Graphiques pour représenter les données de la centrale inertielle
"""

from graphique import Graphique

from tkinter import Tk, Text, BOTH, HORIZONTAL, RAISED, W, N, E, S
from tkinter import Frame

class Graphiques(Frame):
    "Classe pour gérer l'ensemble des graphiques"

    def __init__(self, root, height):
        super().__init__(root, height=height)
        
        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)
        self.rowconfigure(1, weight=1)
        self.rowconfigure(2, weight=1)
        self.rowconfigure(3, weight=1)
        self.rowconfigure(4, weight=1)
        self.rowconfigure(5, weight=1)

        self.graph_x = Graphique(self, text="x")
        self.graph_x.grid(column=0, row=0, sticky=(N, S, E, W))
        self.graph_y = Graphique(self, text="y")
        self.graph_y.grid(column=0, row=1, sticky=(N, S, E, W))
        self.graph_z = Graphique(self, text="z")
        self.graph_z.grid(column=0, row=2, sticky=(N, S, E, W))
        self.graph_alpha = Graphique(self, text="α")
        self.graph_alpha.grid(column=0, row=3, sticky=(N, S, E, W))
        self.graph_beta = Graphique(self, text="β")
        self.graph_beta.grid(column=0, row=4, sticky=(N, S, E, W))
        self.graph_gamma = Graphique(self, text="γ")
        self.graph_gamma.grid(column=0, row=5, sticky=(N, S, E, W))
        
        self.graph_x.init_ui()
        self.graph_y.init_ui()
        self.graph_z.init_ui()
        self.graph_alpha.init_ui()
        self.graph_beta.init_ui()
        self.graph_gamma.init_ui()

    def ajouter_telemetrie(self, t_origine, data):        
        t = (data.t - t_origine) / 1000
        self.graph_x.x1.append(t)
        self.graph_x.y1.append(data.ax)
        self.graph_x.x2.append(t)
        self.graph_x.y2.append(data.vx)
        self.graph_x.x3.append(t)
        self.graph_x.y3.append(data.x)

        self.graph_y.x1.append(t)
        self.graph_y.y1.append(data.ay)
        self.graph_y.x2.append(t)
        self.graph_y.y2.append(data.vy)
        self.graph_y.x3.append(t)
        self.graph_y.y3.append(data.y)

        self.graph_z.x1.append(t)
        self.graph_z.y1.append(data.az)
        self.graph_z.x2.append(t)
        self.graph_z.y2.append(data.vz)
        self.graph_z.x3.append(t)
        self.graph_z.y3.append(data.z)

        self.graph_alpha.x2.append(t)
        self.graph_alpha.y2.append(data.valpha)
        self.graph_alpha.x3.append(t)
        self.graph_alpha.y3.append(data.alpha)

        self.graph_beta.x2.append(t)
        self.graph_beta.y2.append(data.vbeta)
        self.graph_beta.x3.append(t)
        self.graph_beta.y3.append(data.beta)

        self.graph_gamma.x2.append(t)
        self.graph_gamma.y2.append(data.vgamma)
        self.graph_gamma.x3.append(t)
        self.graph_gamma.y3.append(data.gamma)

    def maj(self):
        self.graph_x.maj()
        self.graph_y.maj()
        self.graph_z.maj()
        self.graph_alpha.maj()
        self.graph_beta.maj()
        self.graph_gamma.maj()


    def effacer(self):        
        self.graph_x.effacer()
        self.graph_y.effacer()
        self.graph_z.effacer()
        self.graph_alpha.effacer()
        self.graph_beta.effacer()
        self.graph_gamma.effacer()

    