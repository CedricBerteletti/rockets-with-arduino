# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Point d'entrée pour le programme du poste de contrôle de la fusée
"""

from tkinter import Tk
from ecran_principal import EcranPrincipal

# Constantes
LARGEUR_ECRAN = 900
HAUTEUR_ECRAN = 600



def main(args):
    fenetre = Tk("")
    fenetre.title("Centre de Contrôle et de Télémétrie")
    fenetre.geometry(str(LARGEUR_ECRAN) + "x" + str(HAUTEUR_ECRAN))
    principal = EcranPrincipal()
    fenetre.mainloop()



print("Programme principal")
if __name__ == "__main__":
    from sys import argv
    try:
        main(argv)
    except KeyboardInterrupt:
        pass