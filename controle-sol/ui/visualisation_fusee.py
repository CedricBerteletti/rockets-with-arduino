# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Widget QT pour la visualisation 3D de la fusée
"""


import services.settings as settings
import numpy as np
from PyQt6.QtGui import QVector3D
from pyqtgraph.opengl import GLViewWidget, MeshData, GLMeshItem, GLGridItem
from stl import Mesh, stl



class VisualisationFusee(GLViewWidget):
    "Représentation en 3D de l'orientation et de l'état de la fusée"
    "Documentation : https://pyqtgraph.readthedocs.io/en/latest/api_reference/3dgraphics/index.html"


    def __init__(self):
        super().__init__()        
        self.setMinimumSize(640, 480)
        self.rocket_mesh_data = None
        self.rocket_mesh = None
        self.grid_size = settings.get_int("visu3d.grid_size")
        self.camera_center = np.array([0, 0, 0])
        self.camera_distance = settings.get_int("visu3d.camera_distance")
        self.init_scene()

    
    def init_scene(self):
        mesh = Mesh.from_file("../modeles-3d/200_etage2_base.stl")

        points = mesh.points.reshape(-1, 3)
        faces = np.arange(points.shape[0]).reshape(-1, 3)

        # Calculer le centre du mesh
        center = np.mean(points, axis=0)
        self.camera_center = QVector3D(center[0], center[1], center[2])

        self.rocket_mesh_data = MeshData(vertexes=points, faces=faces)
        self.rocket_mesh = GLMeshItem(meshdata=self.rocket_mesh_data, smooth=True, drawFaces=False, drawEdges=True, edgeColor=(0, 1, 0, 1))
        self.addItem(self.rocket_mesh)

        # Ajouter une grille
        if self.grid_size:
            grid = GLGridItem()
            grid.setSize(self.grid_size, self.grid_size)
            grid.setSpacing(5, 5)
            self.addItem(grid)

        # Centrer la caméra sur le mesh
        self.setCameraPosition(pos=self.camera_center, distance=self.camera_distance)
    
    def maj_orientation(self, alpha, beta, gamma):
        "Mettre à jour l'orientation de la fusée"
        self.rocket_mesh.resetTransform()
        self.rocket_mesh.rotate(alpha, 1, 0, 0)
        self.rocket_mesh.rotate(beta, 0, 1, 0)
        self.rocket_mesh.rotate(gamma, 0, 0, 1)
    
    def raz(self):
        "Remettre à 0 l'orientation de la fusée"
        self.rocket_mesh.resetTransform()
        self.rocket_mesh.rotate(0.0, 1, 0, 0)
        self.rocket_mesh.rotate(0.0, 0, 1, 0)
        self.rocket_mesh.rotate(0.0, 0, 0, 1)
