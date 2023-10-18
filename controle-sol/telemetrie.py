# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Gestion du thread d'acquisition des données télémétriques de la fusée
"""

import functools
import logging
import math
import threading
from time import sleep, time

from connexion import Connexion


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
        self.actif = False
        self.debug = False

    def stop(self):
        self.actif = False

    def startDebug(self):
        self.debug = True
        self.start()

    
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
        logging.info("Lancement du thread d'aquisition des données")
        if not self.debug:
            while self.actif:
                logging.debug("Essai lecture données Arduino")
                str = self.connexion.recevoir()
                if str:
                    if str.find(", IMU_DATA,") != -1:
                        self.logsImu.append(str)
                        self.tempLogsImu.append(str)
                    else:
                        self.logs.append(str)
                        self.tempLogs.append(str)
        else:
            start = time()
            while self.actif:
                t = time() - start
                t_ms = int(1000*t)
                value = math.sin(math.pi*t)
                str = f"00000, IMU_DATA, \"t, accX, accY, accZ, vAlpha, vBeta, vGamma\", \"{t_ms}, {value}, {value}, {value}, {value}, {value}, {value}\""
                self.logsImu.append(str)
                self.tempLogsImu.append(str)
                sleep(0.1)

        logging.info("Fin du thread d'aquisition des données")


    

