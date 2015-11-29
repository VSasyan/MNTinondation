#include "bornes.h"

#include <recentreur.h>

Bornes::Bornes() {
    this->min = qglviewer::Vec(0,0,10000);
    this->max = qglviewer::Vec(0,0,-10000);
    this->pas = 0;
}

void Bornes::chargerLignes(QStringList Ligne_1, QStringList Ligne_2, QStringList Ligne_n) {
    if (Ligne_1[0] != Ligne_2[0]) { // Pas en X :
        this->pas = abs(Ligne_1[0].toFloat() - Ligne_2[0].toFloat());
    } else { // Pas en Y :
        this->pas = abs(Ligne_1[1].toFloat() - Ligne_2[1].toFloat());
    }
    this->max.x = std::max(Ligne_1[0].toFloat(), Ligne_n[0].toFloat());
    this->min.x = std::min(Ligne_1[0].toFloat(), Ligne_n[0].toFloat());
    this->max.y = std::max(Ligne_1[1].toFloat(), Ligne_n[1].toFloat());
    this->min.y = std::min(Ligne_1[1].toFloat(), Ligne_n[1].toFloat());
}

qglviewer::Vec Bornes::getMax() {
    return max;
}

qglviewer::Vec Bornes::getMin() {
    return min;
}

qglviewer::Vec Bornes::getCenter() {
    return (max + min) / 2;
}

qreal Bornes::getRadius() {
    qreal norme = (max - min).norm();
    return norme + sqrt(norme);
}

int Bornes::calnbX() {
    return int((max.x - min.x) / pas) + 1;
}

int Bornes::calnbY() {
    return int((max.y - min.y) / pas) + 1;
}

void Bornes::majZ(qreal z) {
    if (min.z > z) {min.z = z;}
    if (max.z < z) {max.z = z;}
}

void Bornes::recentrer(Recentreur recentreur) {
    this->min -= recentreur.delta;
    this->max -= recentreur.delta;

    this->min /= recentreur.pas;
    this->max /= recentreur.pas;
    this->pas /= recentreur.pas;
}
