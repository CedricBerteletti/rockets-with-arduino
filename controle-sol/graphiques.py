# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Graphiques pour représenter les données de la centrale inertielle
"""

import logging
import time

from graphique import Graphique

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt

from tkinter import Tk, Text, BOTH, HORIZONTAL, RAISED, W, N, E, S
from tkinter import Frame

from PyQt5.QtWidgets import QWidget



class BaseGraphiquesIntegres(QWidget):
    "Classe abstraite pour la gestion des graphiques"

    def __init__(self, root):
        super().__init__()


    def ajouter_telemetrie(self, t_origine, data):
        logging.debug("Ajouter des données")
        t = (data.t - t_origine) / 1000
        self.t.append(t)

        self.ax.append(data.ax)
        self.vx.append(data.vx)
        self.x.append(data.x)

        self.ay.append(data.ay)
        self.vy.append(data.vy)
        self.y.append(data.y)

        self.az.append(data.az)
        self.vz.append(data.vz)
        self.z.append(data.z)

        self.valpha.append(data.valpha)
        self.alpha.append(data.alpha)
        
        self.vbeta.append(data.vbeta)
        self.beta.append(data.beta)
        
        self.vgamma.append(data.vgamma)
        self.gamma.append(data.gamma)


    def maj(self):        
        logging.debug("MAJ les données")


    def effacer(self):        
        logging.debug("Effacer les données")
        self.t = []

        self.ax = []
        self.vx = []
        self.x = []

        self.ay = []
        self.vy = []
        self.y = []

        self.az = []
        self.vz = []
        self.z = []
        
        self.valpha = []
        self.alpha = []
        
        self.vbeta = []
        self.beta = []
        
        self.vgamma = []
        self.gamma = [] 







class GraphiquesIntegres(BaseGraphiquesIntegres):
    "Classe pour gérer l'ensemble des graphiques dans un seul widget QT"

    def __init__(self, root):
        super().__init__(root)
        self.init_plots()

    def init_plots(self):
        # TODO

        self.effacer()


    def maj(self):
        tstart = time.time()

        super().maj()
        # TODO

        if time.time() > tstart:
            logging.debug(f"FPS = {1/(time.time()-tstart)}")


    def effacer(self):
        super().effacer()


















    