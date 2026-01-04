use <commun.scad>

include <constantes.scad>

module boitier() {
    union() {
        // Boitier externe
        difference() {
            cube([longueur, largeur, hauteur], center = true);
            union() {
                translate ([0, 0, epaisseur]) cube([longueur-2*epaisseur, largeur-2*epaisseur, hauteur-epaisseur], center = true);
                
                // Trous douilles
                translate([0, -largeur/5, -hauteur/2+20])
                rotate([0, 90, 0])
                cylinder(h=longueur+jeu, r1=2.5+jeu, r2=2.5+jeu, center=true, $fn=30);
                translate([0, -largeur/5, -hauteur/2+40])
                rotate([0, 90, 0])
                cylinder(h=longueur+jeu, r1=2.5+jeu, r2=2.5+jeu, center=true, $fn=30);
                
                
                translate([longueur/2, largeur/5, -hauteur/6])
                cube([longueur/2+jeu, 9, 3], center=true);
            }
        }
        
        // Fixation couvercle
        translate ([longueur/2-2*epaisseur, largeur/2-2*epaisseur, 0]) fixation_couvercle();
        translate ([-longueur/2+2*epaisseur, largeur/2-2*epaisseur, 0]) fixation_couvercle();
        translate ([longueur/2-2*epaisseur, -largeur/2+2*epaisseur, 0]) fixation_couvercle();
        translate ([-longueur/2+2*epaisseur, -largeur/2+2*epaisseur, 0]) fixation_couvercle();
        
        // Support circuit imprimé
        translate([-longueur/2+44/2+epaisseur+18, largeur/2-39/2-epaisseur-8, -hauteur/2+epaisseur+4]) pieds_support_rect(44, 39, 10, 8);
        
        relai_trans_x = longueur/2-28/2-epaisseur-15;
        relai_trans_y = largeur/2-20/2-epaisseur-14;
        relai_trans_z = -hauteur/2+epaisseur+15;
        // Relai commande fusée
        translate([relai_trans_x, relai_trans_y, relai_trans_z]) relai_arduino_support(30);
        
        // Relai commande allumage
        translate([relai_trans_x, -relai_trans_y, relai_trans_z+10]) relai_arduino_support(50);
        
        // Relai puissance allumage
    }
}

module fixation_couvercle() {
    difference () {
        translate ([0, 0, -epaisseur/2-jeu/2]) cube([2*epaisseur, 2*epaisseur, hauteur-epaisseur-jeu], center = true);
        translate ([0, 0, hauteur/2-10]) m2_trou_insert();
    }
}

boitier();
