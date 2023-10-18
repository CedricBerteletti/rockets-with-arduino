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



class DebugGraphiquesIntegres(Frame):
    "Classe abstraite pour la gestion des graphiques"

    def __init__(self, root, height):
        super().__init__(root, height=height)    

    def ajouter_telemetrie(self, t_origine, data):
        logging.debug("Ajouter des données")

    def maj(self):        
        logging.debug("MAJ les données")


    def effacer(self):        
        logging.debug("Effacer les données")


class GraphiquesIntegres(Frame):
    "Classe pour gérer l'ensemble des graphiques dans un seul canevas FigureCanvasTkAgg"

    def __init__(self, root, height):
        super().__init__(root, height=height)
        self.init_plots()

    def init_plots(self):
        self.fig = plt.figure()
        self.fig.set_figheight(15)
        self.fig.set_figwidth(20)
        self.gs = self.fig.add_gridspec(3, 6, hspace=0, wspace=0)
        #self.fig, self.plots = plt.subplots(3, 5)
        self.canvas = FigureCanvasTkAgg(self.fig, master=self)
        self.canvas.get_tk_widget().pack()

        self.effacer()

    

    def ajouter_telemetrie(self, t_origine, data):
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
        tstart = time.time()

        self.plots[0,0].plot(self.t, self.ax, color="red")
        self.plots[1,0].plot(self.t, self.vx, color="blue")
        self.plots[2,0].plot(self.t, self.x, color="green")

        self.plots[0,1].plot(self.t, self.ay, color="red")
        self.plots[1,1].plot(self.t, self.vy, color="blue")
        self.plots[2,1].plot(self.t, self.y, color="green")
        
        self.plots[0,2].plot(self.t, self.az, color="red")
        self.plots[1,2].plot(self.t, self.vz, color="blue")
        self.plots[2,2].plot(self.t, self.z, color="green")

        self.plots[1,3].plot(self.t, self.valpha, color="blue")
        self.plots[2,3].plot(self.t, self.alpha, color="green")
        
        self.plots[1,4].plot(self.t, self.vbeta, color="blue")
        self.plots[2,4].plot(self.t, self.beta, color="green")

        self.plots[1,5].plot(self.t, self.vgamma, color="blue")
        self.plots[2,5].plot(self.t, self.gamma, color="green")
        
        self.canvas.draw()

        logging.debug(f"FPS = {1/(time.time()-tstart)}")


    def effacer(self):        
        self.fig.clf()

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

        self.plots = self.gs.subplots(sharex=True, sharey='row')
        
        self.plots[0,0].set(xlabel='x', ylabel='a (g)')
        self.plots[1,0].set(xlabel='x', ylabel='v (m/s ou °/s)')
        self.plots[2,0].set(xlabel='x', ylabel='p (m ou °)')

        # self.plots[0,1].set(xlabel='y', ylabel='a (g)')
        # self.plots[1,1].set(xlabel='y', ylabel='v (m/s ou °/s)')
        #self.plots[2,1].set(xlabel='y', ylabel='p (m ou °)')
        self.plots[2,1].set(xlabel='y')

        # self.plots[0,2].set(xlabel='z', ylabel='a (g)')
        # self.plots[1,2].set(xlabel='z', ylabel='v (m/s ou °/s)')
        #self.plots[2,2].set(xlabel='z', ylabel='p (m ou °)')
        self.plots[2,2].set(xlabel='z')

        # self.plots[0,3].set(xlabel='α', ylabel='a (g)')
        # self.plots[1,3].set(xlabel='α', ylabel='v (m/s ou °/s)')
        #self.plots[2,3].set(xlabel='α', ylabel='p (m ou °)')
        self.plots[2,3].set(xlabel='α')

        # self.plots[0,4].set(xlabel='β', ylabel='a (g)')
        # self.plots[1,4].set(xlabel='β', ylabel='v (m/s ou °/s)')
        #self.plots[2,4].set(xlabel='β', ylabel='p (m ou °)')
        self.plots[2,4].set(xlabel='β')

        # self.plots[0,5].set(xlabel='γ', ylabel='a (g)')
        # self.plots[1,5].set(xlabel='γ', ylabel='v (m/s ou °/s)')
        #self.plots[2,5].set(xlabel='γ', ylabel='p (m ou °)')
        self.plots[2,5].set(xlabel='γ')

        # for plot in self.plots:
        #     plot.label_outer()





# class GraphiquesIntegres(Frame):
#     "Classe pour gérer l'ensemble des graphiques dans un seul canevas FigureCanvasTkAgg"

#     def __init__(self, root, height):
#         super().__init__(root, height=height)
#         self.init_plots()

#     def init_plots(self):
#         self.fig = plt.figure()
#         self.fig.set_figheight(15)
#         self.fig.set_figwidth(20)
#         self.gs = self.fig.add_gridspec(3, 6, hspace=0, wspace=0)
#         #self.fig, self.plots = plt.subplots(3, 5)
#         self.canvas = FigureCanvasTkAgg(self.fig, master=self)
#         self.canvas.get_tk_widget().pack()

#         self.effacer()

    

#     def ajouter_telemetrie(self, t_origine, data):
#         t = (data.t - t_origine) / 1000
#         self.t.append(t)

#         self.ax.append(data.ax)
#         self.vx.append(data.vx)
#         self.x.append(data.x)

#         self.ay.append(data.ay)
#         self.vy.append(data.vy)
#         self.y.append(data.y)

#         self.az.append(data.az)
#         self.vz.append(data.vz)
#         self.z.append(data.z)

#         self.valpha.append(data.valpha)
#         self.alpha.append(data.alpha)
        
#         self.vbeta.append(data.vbeta)
#         self.beta.append(data.beta)
        
#         self.vgamma.append(data.vgamma)
#         self.gamma.append(data.gamma)


#     def maj(self):
#         tstart = time.time()

#         self.plots[0,0].plot(self.t, self.ax, color="red")
#         self.plots[1,0].plot(self.t, self.vx, color="blue")
#         self.plots[2,0].plot(self.t, self.x, color="green")

#         self.plots[0,1].plot(self.t, self.ay, color="red")
#         self.plots[1,1].plot(self.t, self.vy, color="blue")
#         self.plots[2,1].plot(self.t, self.y, color="green")
        
#         self.plots[0,2].plot(self.t, self.az, color="red")
#         self.plots[1,2].plot(self.t, self.vz, color="blue")
#         self.plots[2,2].plot(self.t, self.z, color="green")

#         self.plots[1,3].plot(self.t, self.valpha, color="blue")
#         self.plots[2,3].plot(self.t, self.alpha, color="green")
        
#         self.plots[1,4].plot(self.t, self.vbeta, color="blue")
#         self.plots[2,4].plot(self.t, self.beta, color="green")

#         self.plots[1,5].plot(self.t, self.vgamma, color="blue")
#         self.plots[2,5].plot(self.t, self.gamma, color="green")
        
#         self.canvas.draw()

#         logging.debug(f"FPS = {1/(time.time()-tstart)}")


#     def effacer(self):        
#         self.fig.clf()

#         self.t = []

#         self.ax = []
#         self.vx = []
#         self.x = []

#         self.ay = []
#         self.vy = []
#         self.y = []

#         self.az = []
#         self.vz = []
#         self.z = []
        
#         self.valpha = []
#         self.alpha = []
        
#         self.vbeta = []
#         self.beta = []
        
#         self.vgamma = []
#         self.gamma = [] 

#         self.plots = self.gs.subplots(sharex=True, sharey='row')
        
#         self.plots[0,0].set(xlabel='x', ylabel='a (g)')
#         self.plots[1,0].set(xlabel='x', ylabel='v (m/s ou °/s)')
#         self.plots[2,0].set(xlabel='x', ylabel='p (m ou °)')

#         # self.plots[0,1].set(xlabel='y', ylabel='a (g)')
#         # self.plots[1,1].set(xlabel='y', ylabel='v (m/s ou °/s)')
#         #self.plots[2,1].set(xlabel='y', ylabel='p (m ou °)')
#         self.plots[2,1].set(xlabel='y')

#         # self.plots[0,2].set(xlabel='z', ylabel='a (g)')
#         # self.plots[1,2].set(xlabel='z', ylabel='v (m/s ou °/s)')
#         #self.plots[2,2].set(xlabel='z', ylabel='p (m ou °)')
#         self.plots[2,2].set(xlabel='z')

#         # self.plots[0,3].set(xlabel='α', ylabel='a (g)')
#         # self.plots[1,3].set(xlabel='α', ylabel='v (m/s ou °/s)')
#         #self.plots[2,3].set(xlabel='α', ylabel='p (m ou °)')
#         self.plots[2,3].set(xlabel='α')

#         # self.plots[0,4].set(xlabel='β', ylabel='a (g)')
#         # self.plots[1,4].set(xlabel='β', ylabel='v (m/s ou °/s)')
#         #self.plots[2,4].set(xlabel='β', ylabel='p (m ou °)')
#         self.plots[2,4].set(xlabel='β')

#         # self.plots[0,5].set(xlabel='γ', ylabel='a (g)')
#         # self.plots[1,5].set(xlabel='γ', ylabel='v (m/s ou °/s)')
#         #self.plots[2,5].set(xlabel='γ', ylabel='p (m ou °)')
#         self.plots[2,5].set(xlabel='γ')

#         # for plot in self.plots:
#         #     plot.label_outer()




class GraphiquesIndependants(Frame):
    "Classe pour gérer l'ensemble des graphiques de façon indépendante"

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





    