# -*- coding: utf-8 -*-# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Gestion de la connexion WiFi à la fusée
"""


import errno
import logging
import socket
from time import sleep

class Connexion():
    "Classe pour gérer la connexion wifi à la fusée"

    timeout = 1 # en seconde

    def __init__(self):
        self.rocket_ip = ""
        self.rocket_port = 0
        self.actif = False

    def init(self, rocket_ip, rocket_port):
        if self.actif:
            self.fermer()

        self.rocket_ip = rocket_ip;
        self.rocket_port = rocket_port;

        self.socket = socket.socket(socket.AF_INET, # Internet
            socket.SOCK_DGRAM, socket.IPPROTO_UDP) # UDP
        self.socket.connect((self.rocket_ip, self.rocket_port))
        self.socket.setblocking(0)

        self.actif = True

    def envoyer(self, message):
        if self.actif:
            logging.info(f"Envoi {message} à {self.rocket_ip}:{self.rocket_port}")
            bytes_envoyes = self.socket.sendto(message.encode(), (self.rocket_ip, self.rocket_port))
            logging.info(f"{bytes_envoyes} octets envoyés")

    def recevoir(self):
        str = ""
        if self.actif:
            try:
                data, addr = self.socket.recvfrom(255) # taille du buffer
            except socket.error as e:
                err = e.args[0]
                if err == errno.EAGAIN or err == errno.EWOULDBLOCK:
                    # pas d'erreur, simplement pas de nouvelle donnée disponible
                    logging.debug(err)
                    sleep(0.01)
                else:
                    # Erreur réelle
                    logging.error("Erreur lors de la tentative de lecture de l'Arduino : ", e)
            else:
                str = data.decode()
            
            logging.debug(str)
        return str

    def fermer(self):
        self.socket.shutdown(socket.SHUT_RDWR)
        self.socket.close()



