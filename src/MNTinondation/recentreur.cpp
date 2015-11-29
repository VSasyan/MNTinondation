#include "recentreur.h"

Recentreur::Recentreur() {
    this->delta = qglviewer::Vec();
    this->pas = 0;
    this->vector = QVector3D();
}

Recentreur::Recentreur(Bornes bornes) {
    this->delta = (bornes.min + bornes.max) / 2;
    this->delta.z = bornes.min.z;
    this->pas = bornes.pas;
    this->vector = QVector3D(delta.x, delta.y, delta.z);
}
