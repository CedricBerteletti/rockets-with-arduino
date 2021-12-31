# -*- coding: utf-8 -*-

import socket
import errno
from time import sleep

class Connexion():
    "Classe pour gérer la connexion wifi à la fusée"

    def __init__(self):
        self.ip = "192.168.0.50"
        self.port = 23900
        
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

    def envoyer(self, message):
        self.sock_send.sendto(message.encode(), (self.ip, self.port))

    def recevoir(self):
        str = ""
        try:
            data, addr = self.sock_rec.recvfrom(255) # taille du buffer
        except socket.error as e:
            err = e.args[0]
            if err == errno.EAGAIN or err == errno.EWOULDBLOCK:
                # pas d'erreur, simplement pas de nouvelle donnée disponible
                sleep(0.01)
            else:
                # Erreur réelle
                print(e)
        else:
            str = data.decode()
        return str

    def fermer(self):
        self.sock_send.shutdown(socket.SHUT_RDWR)
        self.sock_send.close()
        self.sock_rec.shutdown(socket.SHUT_RDWR)
        self.sock_rec.close()



