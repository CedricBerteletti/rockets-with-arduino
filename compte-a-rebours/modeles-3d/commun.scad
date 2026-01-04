jeu = 0.2;

module m2_trou_vis(hauteur=10) {
    cylinder(h=hauteur, r1=1, r2=1, center=true, $fn=30);
}

module m2_trou_insert(hauteur=10) {
    cylinder(h=hauteur, r1=1.6, r2=1.6, center=true, $fn=30);
}

module m3_trou_vis(hauteur=10) {
    cylinder(h=hauteur, r1=1.5, r2=1.5, center=true, $fn=30);
}

module m3_trou_insert(hauteur=10) {
    cylinder(h=hauteur, r1=2, r2=2, center=true, $fn=30);
}

module relai_arduino_support(hauteur=10) {
    pieds_support_rect(28, 20, hauteur, 8);
}

module pieds_support_rect(longueur=28, largeur=20, hauteur=10, profondeur_trou=10) {
    // La longueur et la largeur correspondent aux entre-axes des trous
    // de fixation
    translate([-longueur/2, -largeur/2, 0]) pied(hauteur, profondeur_trou);
    translate([-longueur/2, largeur/2, 0]) pied(hauteur, profondeur_trou);
    translate([longueur/2, -largeur/2, 0]) pied(hauteur, profondeur_trou);
    translate([longueur/2, largeur/2, 0]) pied(hauteur, profondeur_trou);
}

module pied(hauteur=10, profondeur_trou = 10) {
    difference() {
        cube([8, 8, hauteur], center=true);
        translate([0, 0, (hauteur-profondeur_trou)/2]) m2_trou_insert(profondeur_trou+jeu);
    }
}