#include "triangle.h"

Triangle::Triangle() {
    a = -1;
    b = -1;
    c = -1;
}

Triangle::Triangle(int iA, int iB, int iC) {
    a = iA;
    b = iB;
    c = iC;
}

int Triangle::getA() {
    return this->a;
}

int Triangle::getB() {
    return this->b;
}

int Triangle::getC() {
    return this->c;
}
