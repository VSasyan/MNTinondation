#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "segment3d.h"

class Triangle {

public:
    Triangle();
    Triangle(int iA, int iB, int iC);

    int getA();
    int getB();
    int getC();

protected:
    int a, b, c;

};

#endif // TRIANGLE_H
