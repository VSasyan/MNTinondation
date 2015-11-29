#ifndef BORNES_H
#define BORNES_H

#include <QGLViewer/qglviewer.h>
#include <QStringList>

class Recentreur;

class Bornes {
public:
    Bornes();

    qglviewer::Vec getMax();
    qglviewer::Vec getMin();
    qglviewer::Vec getCenter();
    qreal getRadius();

    int calnbX();
    int calnbY();
    void majZ(qreal z);
    void recentrer(Recentreur recentreur);
    void chargerLignes(QStringList Ligne_1, QStringList Ligne_2, QStringList Ligne_n);

    float pas;
    qglviewer::Vec min, max;
};

#endif // BORNES_H
