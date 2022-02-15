# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Calculs à partir des données de la centrale inertielle
"""

from dataclasses import dataclass


FILTRE_VITESSE_ANGULAIRE_MIN = 0.2 # 0.1 ° par seconde
FILTRE_ACCELERATION_MIN = 0.001 # g (gravité terrestre 9.8 m/s²)

class CentraleInertielle():
    "Classe gérant toutes les données inertielles"

    def __init__(self):
        self.courant = DonneesInertielles()
        self.data_liste = []
        self.delta_ax = 0.0
        self.delta_ay = 0.0
        self.delta_az = 0.0
        self.delta_valpha = 0.0
        self.delta_vbeta = 0.0
        self.delta_vgamma = 0.0
        
    def ajouter_telemetrie(self, log):
        if (log):
            data = DonneesInertielles()
            tokens = log.split("\", \"")
            if(len(tokens[1]) > 1):
                values = tokens[1][:-1].split(", ")

                # Récupération des valeurs de la télémétrie, en tenant compte de la calibration
                data.t = int(values[0])
                data.ax = float(values[1]) + self.delta_ax
                data.ay = float(values[2]) + self.delta_ay
                data.az = float(values[3]) + self.delta_az
                data.valpha = float(values[4]) + self.delta_valpha
                data.vbeta = float(values[5]) + self.delta_vbeta
                data.vgamma = float(values[6]) + self.delta_vgamma

                # Pour éviter la dérive des gyroscopes, on filtre les trop petites vitesses angulaires
                if (abs(data.valpha) < FILTRE_VITESSE_ANGULAIRE_MIN):
                    data.valpha = 0.0
                if (abs(data.vbeta) < FILTRE_VITESSE_ANGULAIRE_MIN):
                    data.vbeta = 0.0
                if (abs(data.vgamma) < FILTRE_VITESSE_ANGULAIRE_MIN):
                    data.vgamma = 0.0

                if (self.courant.t > 0):
                    # Il ne s'agit pas de la première donnée reçue, on peut donc calculer
                    # l'orientation et la position
                    temps_ecoule = float(data.t - self.courant.t) / 1000.0
                    data.alpha = self.courant.alpha + data.valpha * temps_ecoule
                    data.beta = self.courant.beta + data.vbeta * temps_ecoule
                    data.gamma = self.courant.gamma + data.vgamma * temps_ecoule

                # Mise à jour de la liste
                self.courant = data
                self.data_liste.append(data)

    def effacer_donnees(self):
        "Efface toutes les données stockées, mais ne modifie pas la calibration"
        self.data_liste = []
        self.courant = DonneesInertielles()

    def calibrer(self):
        "Calibre les accéléromètres et les gyroscopes par rapport aux données stockées, mais ne modifie pas ces dernières"
        self.delta_ax = 0.0
        self.delta_ay = 0.0
        self.delta_az = 0.0
        self.delta_valpha = 0.0
        self.delta_vbeta = 0.0
        self.delta_vgamma = 0.0
        n = 0
        
        # Calcul de la déviation sur tous les échantillons
        for data in self.data_liste:
            self.delta_ax = self.delta_ax + data.ax
            self.delta_ay = self.delta_ay + data.ay
            self.delta_az = self.delta_az + data.az
            self.delta_valpha = self.delta_valpha + data.valpha
            self.delta_vbeta = self.delta_vbeta + data.vbeta
            self.delta_vgamma = self.delta_vgamma + data.vgamma
            n = n+1
        
        # Déviation moyenne en fonction du nombre n d'échantillons
        self.delta_ax = - self.delta_ax / n
        self.delta_ay = - self.delta_ay / n
        self.delta_az = - self.delta_az / n
        self.delta_valpha = - self.delta_valpha / n
        self.delta_vbeta = - self.delta_vbeta / n
        self.delta_vgamma = - self.delta_vgamma / n

        print("CALIBRATION ", self.delta_ax, self.delta_ay, self.delta_az, self.delta_valpha, self.delta_vbeta, self.delta_vgamma)






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