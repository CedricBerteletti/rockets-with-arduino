# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Fichier pour le chargement des configurations
"""

import configparser

def init():
    global config
    config = configparser.ConfigParser(inline_comment_prefixes="#")
    config.read("controle-sol.ini")

def get(str):
    global config
    return config.get("DEFAULT", str)

def get_bool(str):
    global config
    return config.getboolean("DEFAULT", str)

def get_int(str):
    global config
    return config.getint("DEFAULT", str)