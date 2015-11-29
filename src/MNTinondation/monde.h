#ifndef MONDE_H
#define MONDE_H

#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector3D>
#include <QGLBuffer>
#include <QStringList>
#include <algorithm>
#include <iostream>

#include "bornes.h"
#include "recentreur.h"
#include "polyligne.h"

class Monde {

protected:
    QVector<QVector3D> pointArray;
    QVector<Polyligne> polyligneArray;
    Bornes bornes;
    Recentreur recentreur;
    int nbX, nbY;
    bool VBO;

    void ajouterTriangle(int i, int x, int y, int z);

public:
    QVector<GLfloat> vertexArray;
    QVector<GLuint> indiceArray;
    QVector<GLfloat> colorArray;
    Monde();
    ~Monde();

    void setBornes(QStringList Ligne_1, QStringList Ligne_2, QStringList Ligne_n);
    void setNbX(int nbX);
    void setNbY(int nbY);
    void setVBO(bool VBO);
    void resetPolyligne();

    int getNbPoints();
    int getNbTriangles();
    QVector3D getPoint(int i);
    QVector<Polyligne> getPolyligneArray();
    Bornes getBornes();
    bool getVBO();
    QString getSizes();
    Recentreur *getRecentreur();

    void genererBuffer();
    void libererBuffer();
    void setSizeIndiceArray();
    void setSizeColorArray();
    void trianguler();
    void segmenter(qreal d);
    void ajouterPoint(QStringList point);
    void trierTriangle(int i, int a, int b, int c);
    void recentrer();

    QString sauverPolyligne(QString src);

    QGLBuffer vertexBuffer;
    QGLBuffer indiceBuffer;
    QGLBuffer colorBuffer;
};

#endif // MONDE_H
