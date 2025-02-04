/*
 * --------------------------------------------------------------------------
 * Gestion d'un servomoteur
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#include "Servomoteur.hpp"

int positionEnAngle(int position, int offset, float slope) {
  return position*slope + offset;
}

int angleEnPosition(int angle, int offset, float slope) {
  return (angle - offset) / slope;
}


const char Servomoteur::MODULE_SERVO[] = "SERVO";

Servomoteur::Servomoteur(Logger &logger):logger(logger) {
}

void Servomoteur::init(int i, int b) {
  id = i;
  broche = b;
  servo.attach(broche);
  logStatut();
}

void Servomoteur::setDecalage(int d) {
  decalage = d;
}

void Servomoteur::setPente(float p) {
  pente = p;
}

void Servomoteur::angle(int a) {
  servo.write(a);
}

void Servomoteur::position(int p) {
  angle(positionEnAngle(p, decalage, pente));
}

void Servomoteur::logStatut() {
  sprintf(strTmp, "Servo: %i | Angle: %i | Décalage: %i | Pente: %f", id, servo.read(), decalage, pente);
  logger.log(MODULE_SERVO, "STATUT", strTmp);
}
