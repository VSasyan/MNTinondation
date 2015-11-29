#ifndef MONDEVIEWER_H
#define MONDEVIEWER_H

#define CIEL  0
#define SOL   1
#define EAU   2
#define HERBE   3
#define TEXTURES   4

#include <QGLViewer/qglviewer.h>
#include <QOpenGLTexture>
#include "monde.h"
#include "vagues.h"

class MondeViewer : public QGLViewer {
public:
    MondeViewer(QWidget *parent);
    Monde monde;

    void setVitesse(float v);
    void setPrecision(float p);
    void setHSol(float h);
    void setHEau(float h);
    void setAffSol(bool newVal);
    void setAffEau(bool newVal);
    void setAffVagues(bool newVal);
    void setAffMNT(bool newVal);
    void setAffCiel(bool newVal);
    void setAffTexturesMNT(bool newVal);

    float getVitesse();
    float getPrecision();
    float getHSol();
    float getHEau();
    bool getAffSol();
    bool getAffEau();
    bool getAffVagues();
    bool getAffMNT();
    bool getAffCiel();
    bool getAffTexturesMNT();

    void segmenter();
    void monteeDesEaux();
    void resetAnimation();
    void resetView();
    void lancerAnimation();
    void stopperAnimation();

protected :
    QTime heureInnondation;
    bool eauMonte, affSol, affEau, affMNT, affVagues, affCiel, affTexturesMNT;
    GLfloat hEau, hSol, vitesse, precision, radius, tailleEau;
    qglviewer::Vec center;
    int repeat;
    QOpenGLTexture *textures[TEXTURES];
    Vagues vagues;

    virtual void animate();
    virtual void draw();
    virtual void init();
    virtual QString helpString() const;

    bool loadSkyTexture(int side, QString src);

    void drawLight();
    void drawSol();
    void drawCiel();
    void drawMNT();
    void drawMNT_VBO();
    void drawEau();
    void drawEauStatique();
    void drawIsoligne();
};

#endif // MONDEVIEWER_H
