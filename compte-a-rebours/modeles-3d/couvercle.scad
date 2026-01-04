use <commun.scad>

include <constantes.scad>

longueur_couvercle = longueur-2*epaisseur;
largeur_couvercle = largeur-2*epaisseur;

module couvercle() {
    union() {
        difference() {
            cube([longueur_couvercle-2*jeu, largeur_couvercle-2*jeu, 3], center = true);
            
            // Trous vis
            translate ([longueur/2-2*epaisseur, largeur/2-2*epaisseur, 0]) m2_trou_vis();
            translate ([-longueur/2+2*epaisseur, largeur/2-2*epaisseur, 0]) m2_trou_vis();
            translate ([longueur/2-2*epaisseur, -largeur/2+2*epaisseur, 0]) m2_trou_vis();
            translate ([-longueur/2+2*epaisseur, -largeur/2+2*epaisseur, 0]) m2_trou_vis();
            
            // Trou bouton
            translate ([longueur_couvercle/6, 0, 0]) cylinder(h=2*epaisseur, r1=3.5, r2=3.5, center=true, $fn=30);
            
            // Trou LED
            translate ([-longueur_couvercle/6, 0, 0]) cylinder(h=2*epaisseur, r1=3, r2=3, center=true, $fn=30);
            
            // Trou carte
            translate ([-longueur_couvercle/2+40, largeur_couvercle/2-6, 0]) cube([60, 2, 2*epaisseur], center = true);
        }
     
        // Plaque carte
        h = 30;
        translate ([-longueur_couvercle/2+40, largeur_couvercle/2-9, -15]) cube([60, 3, h], center = true);
    }
}

couvercle();