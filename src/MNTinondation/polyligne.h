#ifndef POLYLIGNE_H
#define POLYLIGNE_H

#include <QVector>
#include <QGLViewer/qglviewer.h>
#include <QVector3D>

#include <recentreur.h>

class Polyligne
{
public:
     Polyligne();
     Polyligne(qglviewer::Vec pointE, qglviewer::Vec pointF);
    ~Polyligne();

     int getSize();
     qglviewer::Vec getPoint(int i, Recentreur recentreur);
     QList<qglviewer::Vec> *getPoints();
     bool testerAjout(qglviewer::Vec pointE, qglviewer::Vec pointF);
     void ajoutPoints(qglviewer::Vec pointE, qglviewer::Vec pointF);
     void fusionnerPoly(QList<qglviewer::Vec> *points2);


protected:
    QList<qglviewer::Vec>points;
};

#endif // POLYLIGNE_H
