# -*- coding: utf-8 -*-# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Gestion de la connexion WiFi à la fusée
"""


import errno
import logging
import select
import socket
from time import sleep

class Connexion():
    "Classe pour gérer la connexion wifi à la fusée"

    timeout = 1 # en seconde

    def __init__(self):
        self.ip = ""
        self.port = 0
        self.actif = False

    def init(self, ip, port):
        self.ip = ip
        self.port = port
        self.sock_send = socket.socket(socket.AF_INET, # Internet
            socket.SOCK_DGRAM) # UDP
        local_ip = ""
        self.sock_rec = socket.socket(socket.AF_INET, # Internet
            socket.SOCK_DGRAM) # UDP
        self.sock_rec.bind((local_ip, self.port))
        self.sock_rec.setblocking(0)
        self.actif = True

    def envoyer(self, message):
        if self.actif:
            logging.info(f"Envoi {message} à {self.ip}:{self.port}")
            bytes_envoyes = self.sock_send.sendto(message.encode(), (self.ip, self.port))
            logging.info(f"{bytes_envoyes} octets envoyés")

    def recevoir(self):
        str = ""
        if self.actif:
            ready = select.select([self.sock_rec], [], [], self.timeout)
            if ready[0]:        
                try:
                    data, addr = self.sock_rec.recv(255) # taille du buffer
                except socket.error as e:
                    err = e.args[0]
                    if err == errno.EAGAIN or err == errno.EWOULDBLOCK:
                        # pas d'erreur, simplement pas de nouvelle donnée disponible
                        sleep(0.01)
                    else:
                        # Erreur réelle
                        logging.error("Erreur lors de la tentative de lecture de l'Arduino : ", e)
                else:
                    logging.debug(str)
                    str = data.decode()
        return str

    def fermer(self):
        self.sock_send.shutdown(socket.SHUT_RDWR)
        self.sock_send.close()
        self.sock_rec.shutdown(socket.SHUT_RDWR)
        self.sock_rec.close()



