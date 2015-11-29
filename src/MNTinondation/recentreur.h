#ifndef RECENTREUR_H
#define RECENTREUR_H

#include "bornes.h"
#include "QVector3D"

class Recentreur {
public:
    Recentreur();
    Recentreur(Bornes bornes);

    qglviewer::Vec delta;
    QVector3D vector;
    float pas;
};

#endif // RECENTREUR_H
