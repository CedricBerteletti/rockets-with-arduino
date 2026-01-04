use <boitier.scad>
use <couvercle.scad>

include <constantes.scad>

boitier();

translate ([0, 0, hauteur + epaisseur]) couvercle();

