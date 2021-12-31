# -*- coding: utf-8 -*-

from connexion import Connexion

class Controleur():
    "Classe pour gérer les commandes envoyées à la fusée"

    def __init__(self, connexion):
        self.connexion = connexion

    def envoyerCommandeBrute(self, commande):
        self.connexion.envoyer(commande)
