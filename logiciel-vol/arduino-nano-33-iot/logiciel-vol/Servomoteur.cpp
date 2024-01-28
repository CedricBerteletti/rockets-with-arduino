/*
 * --------------------------------------------------------------------------
 * Gestion d'un servomoteur
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#include "Servomoteur.hpp"

int positionEnAngle(int position, int offset, float slope) {
  return position*slope + offset + 90;
}

int angleEnPosition(int angle, int offset, float slope) {
  return (angle - offset - 90) / slope;
}


const char Servomoteur::MODULE_SERVO[] = "SERVO";

Servomoteur::Servomoteur() {
}

void Servomoteur::init(int i, int b, Logger *l) {
  id = i;
  broche = b;
  logger = l;
  servo.attach(broche);
}

void Servomoteur::setDecalage(int d) {
  decalage = d;
}

void Servomoteur::setPente(float p) {
  pente = p;
}

void Servomoteur::angle(int a) {
  servo.write(a);  
  logger->log(MODULE_SERVO, "ANGLE", itoa(a, strTmp, 10));
}

void Servomoteur::position(int p) {
  angle(positionEnAngle(p, decalage, pente));
}

void Servomoteur::logStatut() {
  sprintf(strTmp, "Servo: %i | Angle: %i", id, servo.read());
  logger->log(MODULE_SERVO, "STATUT", strTmp);
}
