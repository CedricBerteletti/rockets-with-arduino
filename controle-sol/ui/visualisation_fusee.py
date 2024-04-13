# -*- coding: utf-8 -*-
"""
@author: Cédric Berteletti
Widget QT pour la visualisation 3D de la fusée
"""


from PyQt6.QtWidgets import QWidget


class VisualisationFusee(QWidget):
    "Représentation en 3D de l'orientation et de l'état de la fusée"



# from PyQt6.QtWidgets import QWidget
# from OpenGL.GL import *
# from OpenGL.GLU import *
# from PyQt6 import QtGui
# from PyQt6.QtOpenGL import *


# class VisualisationFusee(QWidget):
#     "Représentation en 3D de l'orientation et de l'état de la fusée"

#     def __init__(self):
#         super().__init__()        
#         self.setMinimumSize(640, 480)

#     def paintGL(self):
#         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
#         glLoadIdentity()
#         glTranslatef(-2.5, 0.5, -6.0)
#         glColor3f( 1.0, 1.5, 0.0 );
#         glPolygonMode(GL_FRONT, GL_FILL);
#         glBegin(GL_TRIANGLES)
#         glVertex3f(2.0,-1.2,0.0)
#         glVertex3f(2.6,0.0,0.0)
#         glVertex3f(2.9,-1.2,0.0)
#         glEnd()
#         glFlush()

#     def initializeGL(self):
#         glClearDepth(1.0)              
#         glDepthFunc(GL_LESS)
#         glEnable(GL_DEPTH_TEST)
#         glShadeModel(GL_SMOOTH)
#         glMatrixMode(GL_PROJECTION)
#         glLoadIdentity()                    
#         gluPerspective(45.0,1.33,0.1, 100.0) 
#         glMatrixMode(GL_MODELVIEW)