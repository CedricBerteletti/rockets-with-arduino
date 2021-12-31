# -*- coding: utf-8 -*-

from connexion import Connexion
import threading


import functools

def synchronized(lock):
    "Synchronization decorator"
    def wrap(f):
        @functools.wraps(f)
        def newFunction(*args, **kw):
            with lock:
                return f(*args, **kw)
        return newFunction
    return wrap

lock = threading.Lock()


class Telemetrie(threading.Thread):
    "Classe pour gérer la collecte des données télémétriques de la fusée"

    logs = []
    tempLogs = []
    logsImu = []
    tempLogsImu = []
    actif = False

    def __init__(self, connexion):
        super().__init__()
        self.connexion = connexion
    
    @synchronized(lock)
    def logSuivant(self):
        if len(self.tempLogs) > 0:
            return self.tempLogs.pop(0)
        else:
            return ""

    @synchronized(lock)
    def logImuSuivant(self):
        if len(self.tempLogsImu) > 0:
            return self.tempLogsImu.pop(0)
        else:
            return ""

    def run(self):
        self.actif = True
        print("Lancement du thread d'aquisition des données")
        while self.actif:
            str = self.connexion.recevoir()
            if str:
                if str.find(", IMU_DATA,") != -1:
                    self.logsImu.append(str)
                    self.tempLogsImu.append(str)
                else:
                    self.logs.append(str)
                    self.tempLogs.append(str)
        print("Fin du thread d'aquisition des données")


    

