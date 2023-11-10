# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Graphiques pour représenter les données de la centrale inertielle
"""

import logging
import time

from PyQt5.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QPushButton, QCheckBox
import pyqtgraph as pg



class BaseGraphiquesIntegres(QWidget):
    "Classe abstraite pour la gestion des données des graphiques"

    def __init__(self, root):
        super().__init__()
        self.effacer()


    def ajouter_telemetrie(self, t_origine, data):
        logging.debug("Ajouter des données")
        t = data.t - t_origine
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
        self.plot_positions = True
        self.plot_angles = True
        self.init_ui()
        self.init_plots()


    def init_ui(self):
        # Layout de l'ensemble des graphiques
        self.layout = QVBoxLayout()
        self.setLayout(self.layout)

        layout_command = QHBoxLayout()
        self.layout.addLayout(layout_command)        

        button = QPushButton("Effacer")
        layout_command.addWidget(button)
        button.clicked.connect(self.effacer)

        self.cb_positions = QCheckBox("Positions")
        self.cb_positions.setChecked(True)
        layout_command.addWidget(self.cb_positions)
        self.cb_positions.stateChanged.connect(self.changer_plot_positions)

        self.cb_angles = QCheckBox("Angles")
        self.cb_angles.setChecked(True)
        layout_command.addWidget(self.cb_angles)
        self.cb_angles.stateChanged.connect(self.changer_plot_angles)

        self.plots_grid = pg.GraphicsLayoutWidget()  # Automatically generates grids with multiple items
        self.layout.addWidget(self.plots_grid)

    def changer_plot_positions(self):
        self.plot_positions = self.cb_positions.isChecked()
        self.init_plots()

    def changer_plot_angles(self):
        self.plot_angles = self.cb_angles.isChecked()
        self.init_plots()


    def init_plots(self):
        self.plots_grid.clear()

        # Configuration des couleurs de tracé
        color1 = (255, 0, 0)
        color2 = (0, 255, 0)
        color3 = (0, 0, 255)
        pen1 = pg.mkPen(color=color1)
        pen2 = pg.mkPen(color=color2)
        pen3 = pg.mkPen(color=color3)

        # Graphiques pour la position
        if self.plot_positions:
            plot_item = self.plots_grid.addPlot(row=0, col=0)
            plot_item.addLegend()
            plot_item.setLabel("left", "x (m) / vx (m/s) / ax (m/s²)")
            self.curve_x = plot_item.plot(name="x", pen=pen1)
            self.curve_vx = plot_item.plot(name="vx", pen=pen2)
            self.curve_ax = plot_item.plot(name="ax", pen=pen3)
            
            plot_item = self.plots_grid.addPlot(row=1, col=0)
            plot_item.addLegend()
            plot_item.setLabel("left", "y (m) / vy (m/s) / ay (m/s²)")
            self.curve_y = plot_item.plot(name="y", pen=pen1)
            self.curve_vy = plot_item.plot(name="vy", pen=pen2)
            self.curve_ay = plot_item.plot(name="ay", pen=pen3)
            
            plot_item = self.plots_grid.addPlot(row=2, col=0)
            plot_item.addLegend()
            plot_item.setLabel("left", "z (m) / vz (m/s) / az (m/s²)")
            self.curve_z = plot_item.plot(name="z", pen=pen1)
            self.curve_vz = plot_item.plot(name="vz", pen=pen2)
            self.curve_az = plot_item.plot(name="az", pen=pen3)

        # Graphiques pour les angles
        if self.plot_angles:
            plot_item = self.plots_grid.addPlot(row=0, col=1)
            plot_item.addLegend()
            #plot_item.setLabel("left", "Angle (°)", color="rgb(255, 0, 0)")
            plot_item.setLabel("left", "Angle (°) / Vit ang (°/s)")
            self.curve_alpha = plot_item.plot(name="α", pen=pen1)
            self.curve_valpha = plot_item.plot(name="vα", pen=pen2)

            plot_item = self.plots_grid.addPlot(row=1, col=1)
            plot_item.addLegend()
            plot_item.setLabel("left", "Angle (°) / Vit ang (°/s)")
            self.curve_beta = plot_item.plot(name="β", pen=pen1)
            self.curve_vbeta = plot_item.plot(name="vβ", pen=pen2)

            plot_item = self.plots_grid.addPlot(row=2, col=1)
            plot_item.addLegend()
            plot_item.setLabel("left", "Angle (°) / Vit ang (°/s)")
            plot_item.setLabel("bottom", "Temps (s)")
            self.curve_gamma = plot_item.plot(name="γ", pen=pen1)
            self.curve_vgamma = plot_item.plot(name="vγ", pen=pen2)




    def maj(self):
        tstart = time.time()

        super().maj()
        
        if self.plot_positions:
            self.curve_x.setData(self.t, self.x)
            self.curve_vx.setData(self.t, self.vx)
            self.curve_ax.setData(self.t, self.ax)
            self.curve_y.setData(self.t, self.y)
            self.curve_vy.setData(self.t, self.vy)
            self.curve_ay.setData(self.t, self.ay)
            self.curve_z.setData(self.t, self.z)
            self.curve_vz.setData(self.t, self.vz)
            self.curve_az.setData(self.t, self.az)

        if self.plot_angles:
            self.curve_alpha.setData(self.t, self.alpha)
            self.curve_valpha.setData(self.t, self.valpha)
            self.curve_beta.setData(self.t, self.beta)
            self.curve_vbeta.setData(self.t, self.vbeta)
            self.curve_gamma.setData(self.t, self.gamma)
            self.curve_vgamma.setData(self.t, self.vgamma)

        if time.time() > tstart:
            logging.debug(f"FPS = {1/(time.time()-tstart)}")


    def effacer(self):
        super().effacer()


















    