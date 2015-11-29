#ifndef POLYLIGNE_H
#define POLYLIGNE_H

#include <QVector>
#include <QGLViewer/qglviewer.h>
#include <QVector3D>

class Polyligne
{
public:
     Polyligne();
     Polyligne(qglviewer::Vec pointE, qglviewer::Vec pointF);
    ~Polyligne();

     QList<qglviewer::Vec> *getPoints();
     bool testerAjout(qglviewer::Vec pointE, qglviewer::Vec pointF);
     bool ajoutPoints(qglviewer::Vec pointE, qglviewer::Vec pointF);
     void fusionnerPoly(QList<qglviewer::Vec> *points2);


protected:
    QList<qglviewer::Vec>points;
};

#endif // POLYLIGNE_H
