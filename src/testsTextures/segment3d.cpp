#include "segment3d.h"

Segment3D::Segment3D() {
    this->a = qglviewer::Vec();
    this->b = qglviewer::Vec();
}

Segment3D::Segment3D(qglviewer::Vec pointA, qglviewer::Vec pointB) {
    this->a = qglviewer::Vec(pointA);
    this->b = qglviewer::Vec(pointB);
}

qglviewer::Vec Segment3D::getA() {
    return this->a;
}

qglviewer::Vec Segment3D::getB() {
    return this->b;
}
