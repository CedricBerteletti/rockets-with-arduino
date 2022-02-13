# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Graphique pour représenter les données de la centrale inertielle
"""

from tkinter import Tk, Text, BOTH, HORIZONTAL, RAISED, W, N, E, S
from tkinter import Frame, Button, Label, LabelFrame, PanedWindow
from tkinter.constants import VERTICAL


class Graphique(LabelFrame):
    "Classe pour gérer un graphique"

    def __init__(self, root, text):
        super().__init__(root, text=text)
        self.init_ui()

    def init_ui(self):
        self.area = Text(self)
        self.area.pack(fill=BOTH, expand=True)