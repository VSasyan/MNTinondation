#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle
{
public:
    Triangle(Point3D A, Point3D B, Point3D C);

protected:
    Point3D A, B, C; //ne sont pas les mm que le constructeur, acces via this

};

#endif // TRIANGLE_H
