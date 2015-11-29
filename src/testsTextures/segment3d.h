#ifndef SEGMENT3D_H
#define SEGMENT3D_H

#include "QVector3D"
#include <QGLViewer/qglviewer.h>

class Segment3D {

public:
    Segment3D();
    Segment3D(qglviewer::Vec pointA, qglviewer::Vec pointB);

    qglviewer::Vec getA();
    qglviewer::Vec getB();

protected:
    qglviewer::Vec a, b;

};

#endif // SEGMENT3D_H
