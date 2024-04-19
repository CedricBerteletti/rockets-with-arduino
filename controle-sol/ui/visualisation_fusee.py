# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Widget QT pour la visualisation 3D de la fusée
"""


import numpy as np
from pyqtgraph.opengl import GLViewWidget, MeshData, GLMeshItem
from stl import Mesh



class VisualisationFusee(GLViewWidget):
    "Représentation en 3D de l'orientation et de l'état de la fusée"
    "Documentation : https://pyqtgraph.readthedocs.io/en/latest/api_reference/3dgraphics/index.html"


    def __init__(self):
        super().__init__()        
        self.setMinimumSize(640, 480)
        self.init_scene()

    
    def init_scene(self):
        stl_mesh = Mesh.from_file("../modeles-3d/etage2_base.stl")

        points = stl_mesh.points.reshape(-1, 3)
        faces = np.arange(points.shape[0]).reshape(-1, 3)

        mesh_data = MeshData(vertexes=points, faces=faces)
        mesh = GLMeshItem(meshdata=mesh_data, smooth=True, drawFaces=False, drawEdges=True, edgeColor=(0, 1, 0, 1))
        self.addItem(mesh)
    
