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

#include "triangle.h"
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

    void ajouterTriangle(int i, int x, int y, int z, int a, int b, int c);

    GLfloat indiceTextures[6][2][6] = {
        {
            {0,0, 0,1, 1,1},
            {0,0, 1,0, 1,1}
        },
        {
            {0,0, 1,1, 0,1},
            {0,0, 1,1, 1,0}
        },
        {
            {0,1, 0,0, 1,1},
            {1,0, 0,0, 1,1}
        },
        {
            {0,1, 1,1, 0,0},
            {1,0, 1,1, 0,0}
        },
        {
            {1,1, 0,1, 0,0},
            {1,1, 1,0, 0,0}
        },
        {
            {1,1, 0,0, 0,1},
            {1,1, 0,0, 1,0}
        }
    };

    /*GLfloat indiceTextures[6][2][6] = {
        {
            {0,0, 1,0, 0,1},
            {1,1, 1,0, 0,1}
        },
        {
            {0,0, 0,1, 1,0},
            {1,1, 0,1, 1,0}
        },
        {
            {0,1, 0,0, 1,0},
            {0,1, 1,1, 1,0}
        },
        {
            {0,1, 1,0, 0,0},
            {0,1, 1,0, 1,1}
        },
        {
            {1,0, 0,1, 0,0},
            {1,0, 0,1, 1,1}
        },
        {
            {1,0, 0,0, 0,1},
            {1,0, 1,1, 0,1}
        }
    };*/

public:
    QVector<GLfloat> vertexArray;
    QVector<GLuint> indiceArray;
    QVector<GLfloat> colorArray;
    QVector<GLfloat> textureArray;
    Monde();
    ~Monde();

    void setBornes(QStringList Ligne_1, QStringList Ligne_2, QStringList Ligne_n);
    void setNbX(int nbX);
    void setNbY(int nbY);
    void setVBO(bool VBO);

    int getNbPoints();
    int getNbTriangles();
    QVector3D getPoint(int i);
    QVector<Polyligne> getPolyligneArray();
    Bornes getBornes();
    bool getVBO();
    QString getSizes();

    void genererBuffer();
    void libererBuffer();
    void setSizeIndiceArray();
    void setSizeColorArray();
    void trianguler();
    void segmenter(qreal d);
    void ajouterPoint(QStringList point);
    void trierTriangle(int i, int a, int b, int c);
    void recentrer();

    QGLBuffer vertexBuffer;
    QGLBuffer indiceBuffer;
    QGLBuffer colorBuffer;
    QGLBuffer textureBuffer;
};

#endif // MONDE_H
