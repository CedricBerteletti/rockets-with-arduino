# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Calculs à partir des données de la centrale inertielle
"""

from dataclasses import dataclass


class CentraleInertielle():
    "Classe gérant toutes les données inertielles"

    def __init__(self):
        self.courant = DonneesInertielles()
        print(self.courant)
        self.data_liste = []
        self.deltaax = 0.0
        self.deltaay = 0.0
        self.deltaaz = 0.0
        self.deltavalpha = 0.0
        self.deltavbeta = 0.0
        self.deltavgamma = 0.0
        
    def ajouter_telemetrie(self, log):
        if(log):
            data = DonneesInertielles()
            tokens = log.split("\", \"")
            if(len(tokens[1]) > 1):
                values = tokens[1][:-1].split(", ")
                # Récupération des valeurs de la télémétrie, en tenant compte de la calibration
                data.t = int(values[0])
                data.ax = float(values[1]) + self.deltaax
                data.ay = float(values[2]) + self.deltaay
                data.az = float(values[3]) + self.deltaaz
                data.valpha = float(values[4]) + self.deltavalpha
                data.vbeta = float(values[5]) + self.deltavbeta
                data.vgamma = float(values[6]) + self.deltavgamma

                # Calcul de l'orientation et de la position
                temps_ecoule = float(data.t - self.courant.t) / 1000.0
                print(data.alpha, self.courant.alpha, data.valpha)
                data.alpha = self.courant.alpha + data.valpha * temps_ecoule
                data.beta = self.courant.beta + data.vbeta * temps_ecoule
                data.gamma = self.courant.gamma + data.vgamma * temps_ecoule

                # Mise à jour de la liste
                self.courant = data
                self.data_liste.append(data)

    def effacer_donnees(self):
        self.data_liste = []
        self.courant = DonneesInertielles()
        self.deltaax = 0.0
        self.deltaay = 0.0
        self.deltaaz = 0.0
        self.deltavalpha = 0.0
        self.deltavabeta = 0.0
        self.deltavagamma = 0.0

    def calibrer(self):
        self.deltaax = 0.0
        self.deltaay = 0.0
        self.deltaaz = 0.0
        self.deltavalpha = 0.0
        self.deltavabeta = 0.0
        self.deltavagamma = 0.0
        n = 0
        
        # Calcul de la déviation sur tous les échantillons
        for data in self.data_liste:
            self.deltaax = self.deltaax + data.ax
            self.deltaay = self.deltaay + data.ay
            self.deltaaz = self.deltaaz + data.az
            self.deltavalpha = self.deltavalpha + data.valpha
            self.deltavbeta = self.deltavbeta + data.vbeta
            self.deltavgamma = self.deltavgamma + data.vgamma
            n = n+1
        
        # Déviation moyenne en fonction du nombre n d'échantillons
        self.deltaax = - self.deltaax / n
        self.deltaay = - self.deltaay / n
        self.deltaaz = - self.deltaaz / n
        self.deltavalpha = - self.deltavalpha / n
        self.deltavbeta = - self.deltavbeta / n
        self.deltavgamma = - self.deltavgamma / n

        print("CALIBRATION ", self.deltaax, self.deltaay, self.deltaaz, self.deltavalpha, self.deltavabeta, self.deltavagamma)






@dataclass
class DonneesInertielles:
    "Classe représentant les données de la centrale inertielle et les valeurs calculées à un instant t donné"
    t : int = 0
    ax : float = 0.0
    ay : float = 0.0
    az : float = 0.0
    vx : float = 0.0
    vy : float = 0.0
    vz : float = 0.0
    x : float = 0.0
    y : float = 0.0
    z : float = 0.0
    valpha : float = 0.0
    vbeta : float = 0.0
    vgamma : float = 0.0
    alpha : float = 0.0
    beta : float = 0.0
    gamma : float = 0.0