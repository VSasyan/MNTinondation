#ifndef VAGUES_H
#define VAGUES_H

#include <QOpenGLTexture>
#include <QGLViewer/qglviewer.h>


class Vagues {
public:
    Vagues();

    void setTexture(QOpenGLTexture *texture);
    void setDefinition(int def);

    bool genererMotif();
    void libererMotif();
    GLuint getMotif();

protected:
    GLuint motif;
    GLfloat A, kSin, kCos, unitee, delta;
    int repeat, definition;
    QTime temps;
    QOpenGLTexture *texture;
    QVector<QVector<QVector<GLfloat> > >hauteur;
};

#endif // VAGUES_H
