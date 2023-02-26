# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Graphique pour représenter les données de la centrale inertielle
"""

import logging

from tkinter import Tk, Text, BOTH, HORIZONTAL, RAISED, W, N, E, S
from tkinter import Canvas, Frame, Button, Label, LabelFrame, PanedWindow
from tkinter.constants import VERTICAL

import matplotlib
matplotlib.use('TkAgg')
import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure


class Graphique(LabelFrame):
    "Classe pour gérer un graphique"

    def __init__(self, root, text):
        super().__init__(root, text=text)

    def init_ui(self):
        width = self.winfo_width()/100
        logging.info(width)
        height = self.winfo_height()/100
        self.fig = Figure(figsize=(75, 5), dpi=75)
        self.canvas = FigureCanvasTkAgg(self.fig, master=self)
        self.canvas.get_tk_widget().pack()
        self.effacer()

    def effacer(self):
        self.fig.clf()

        self.x1 = []
        self.y1 = []
        self.plot1 = self.fig.add_subplot(111)
        self.plot1.set_ylabel("a", fontsize=8)
        self.plot1.set_xlabel("t", fontsize=8)
        
        self.x2 = []
        self.y2 = []
        self.plot2 = self.fig.add_subplot(111)
        self.plot2.set_ylabel("v", fontsize=8)
        self.plot2.set_xlabel("t", fontsize=8)
        
        self.x3 = []
        self.y3 = []
        self.plot3 = self.fig.add_subplot(111)
        self.plot3.set_ylabel("p", fontsize=8)
        self.plot3.set_xlabel("t", fontsize=8)


    def maj(self):
        self.plot1.plot(self.x1, self.y1, color="red", linewidth=1)
        self.plot2.plot(self.x2, self.y2, color="blue", linewidth=1)
        self.plot3.plot(self.x3, self.y3, color="green", linewidth=1)
        self.canvas.draw()

    