#include "vagues.h"

Vagues::Vagues() {
    kSin = 2 * M_PI * 0.05;
    kCos = 2 * M_PI * 0.08;
    unitee = 500;
    A = 0.03f;
    temps.start();

    setDefinition(100);
}

void Vagues::setTexture(QOpenGLTexture *texture) {
    this->texture = texture;
}

void Vagues::setDefinition(int def) {
    this->definition = def;
    delta = 1.0f / GLfloat(definition);

    hauteur = QVector<QVector<QVector<GLfloat> > >(definition + 1);
    for (GLfloat i = 0; i <= definition; i++) {
        hauteur[i] = QVector<QVector<GLfloat> >(definition + 1);
        for (GLfloat j = 0; j <= definition; j++) {
            hauteur[i][j] = QVector<GLfloat>(5);
            hauteur[i][j][2] = delta * i; // x OpenGL
            hauteur[i][j][3] = delta * j; // y OpenGL
            hauteur[i][j][0] = hauteur[i][j][2]; // x source
            hauteur[i][j][1] = hauteur[i][j][3]; // y source
            // On ne calcule pas dz (hauteurEau[i][j][4]) car il dépent du temps !
        }
    }
}

bool Vagues::genererMotif() {
    motif = glGenLists(1);
    if (motif != 0) {
        glNewList(motif, GL_COMPILE);

            //glEnable(GL_BLEND);
            glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
            glDisable(GL_LIGHTING);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_TEXTURE_2D);

            GLfloat pos = GLfloat(temps.elapsed()) / unitee;
            for (int i = 0; i <= definition; i++) {
                for (int j = 0; j <= definition; j++) {
                    hauteur[i][j][4] = A * sin(pos + GLfloat(j) * kSin) + A * cos(pos + GLfloat(i) * kCos);
                }
            }

            //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            texture->bind();
                for (int i = 0; i < definition; i++) {
                    int i_ = i + 1;
                    glBegin(GL_TRIANGLE_STRIP);
                        for (int j = 0; j <= definition; j++) {
                            glTexCoord2d(hauteur[i][j][0], hauteur[i][j][1]);
                            glVertex3f(hauteur[i][j][2], hauteur[i][j][3], hauteur[i][j][4]);

                            glTexCoord2d(hauteur[i_][j][0], hauteur[i_][j][1]);
                            glVertex3f(hauteur[i_][j][2], hauteur[i_][j][3], hauteur[i_][j][4]);
                        }
                    glEnd();
                }
            texture->release();

            glDisable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
            //glDisable(GL_BLEND);

        glEndList();
        return true;
    }
    return false;
}

void Vagues::libererMotif() {
    glDeleteLists(motif, 1);
}

GLuint Vagues::getMotif() {
    return motif;
}
