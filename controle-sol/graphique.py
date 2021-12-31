# -*- coding: utf-8 -*-

from tkinter import Tk, Text, BOTH, HORIZONTAL, RAISED, W, N, E, S
from tkinter import Frame, Button, Label, LabelFrame, PanedWindow
from tkinter.constants import VERTICAL


class Graphique(LabelFrame):
    "Classe pour gérer un graphique"

    def __init__(self, root, text):
        super().__init__(root, text=text)
        self.init_ui()

    def init_ui(self):
        area = Text(self)
        area.pack(fill=BOTH, expand=True)