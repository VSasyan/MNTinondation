#include "mondeviewer.h"
#include <QMessageBox>

using namespace std;

MondeViewer::MondeViewer(QWidget *parent) {
    this->setParent(parent);
    this->monde = Monde();
    this->hSol = 0;
    this->hEau = 0;
    this->vitesse = 0.5;
    this->precision = 0.01;
    this->eauMonte = false;
    this->affSol = true;
    this->affEau = true;
    this->affVagues = true;
    this->affMNT = true;
    this->affCiel = true;
    this->affTexturesMNT = false;
}

void MondeViewer::setVitesse(float v) {
    this->vitesse = v;
}

void MondeViewer::setPrecision(float p) {
    this->precision = p;
}

void MondeViewer::setHSol(float h) {
    this->hSol = h;
    this->setHEau(h);
}

void MondeViewer::setHEau(float h) {
    this->hEau = h;
}

void MondeViewer::setAffSol(bool newVal) {
    this->affSol = newVal;
}

void MondeViewer::setAffEau(bool newVal) {
    this->affEau = newVal;
}

void MondeViewer::setAffVagues(bool newVal) {
    this->affVagues = newVal;
}

void MondeViewer::setAffMNT(bool newVal) {
    this->affMNT= newVal;
}

void MondeViewer::setAffCiel(bool newVal) {
    this->affCiel = newVal;
}

void MondeViewer::setAffTexturesMNT(bool newVal) {
    this->affTexturesMNT = newVal;
}

float MondeViewer::getVitesse() {
    return this->vitesse;
}

float MondeViewer::getPrecision() {
    return this->precision;
}

float MondeViewer::getHSol() {
    return this->hSol;
}

float MondeViewer::getHEau() {
    return this->hEau;
}

bool MondeViewer::getAffSol() {
    return this->affSol;
}

bool MondeViewer::getAffEau() {
    return this->affEau;
}

bool MondeViewer::getAffVagues() {
    return this->affVagues;
}

bool MondeViewer::getAffMNT() {
    return this->affMNT;
}

bool MondeViewer::getAffCiel() {
    return this->affCiel;
}

bool MondeViewer::getAffTexturesMNT() {
    return this->affTexturesMNT;
}

void MondeViewer::segmenter() {
    this->monde.segmenter(this->hEau);
}

void MondeViewer::monteeDesEaux() {
    if (heureInnondation.isValid()) {
        GLfloat delta = this->vitesse * heureInnondation.elapsed() / 1000;
        if (delta > precision) {
            this->hEau += delta;
            //this->monde.segmenter(this->hEau);
            if (this->hEau > monde.getBornes().max.z + 1) {stopperAnimation();}
            else {heureInnondation.start();}
        }
    } else {heureInnondation.start();}
}

void MondeViewer::resetAnimation() {
    this->hEau = this->hSol;
    updateGL();
}

void MondeViewer::lancerAnimation() {
    this->heureInnondation.start();
    this->eauMonte = true;
}

void MondeViewer::stopperAnimation() {
    this->eauMonte = false;
}

void MondeViewer::animate() {
    if (eauMonte) {monteeDesEaux();}
}

void MondeViewer::resetView() {
    // Light setup
    /*glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // Light default parameters
    const GLfloat light_ambient[4]  = {1.0, 1.0, 1.0, 1.0};
    const GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};
    const GLfloat light_diffuse[4]  = {1.0, 1.0, 1.0, 1.0};

    glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, 3.0);
    glLightf( GL_LIGHT1, GL_SPOT_CUTOFF,   10.0);
    glLightf( GL_LIGHT1, GL_CONSTANT_ATTENUATION,  1.0f);
    glLightf( GL_LIGHT1, GL_LINEAR_ATTENUATION,    0.0f);
    glLightf( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse);*/

    // Load textures :
    this->loadTexture(0, "../../data/textures/ciel.png");
    this->loadTexture(1, "../../data/textures/sol.png");
    this->loadTexture(2, "../../data/textures/eau2.jpg");
    this->loadTexture(3, "../../data/textures/texture-herbe.png");

    //restoreStateFromFile();
    if (monde.getVBO()) {monde.genererBuffer();}
    //setSceneBoundingBox(monde.getBornes().min, monde.getBornes().max);
    this->centre = monde.getBornes().getCenter();
    this->rayon = monde.getBornes().getRadius();
    this->repetition = sqrt(rayon);
    setSceneCenter(centre);
    setSceneRadius(rayon);
    this->camera()->setPosition(qglviewer::Vec(0, 0, rayon-10));
    this->camera()->lookAt(qglviewer::Vec(0, 0, 0));
    showEntireScene();

    vagues = Vagues();
    vagues.setTexture(textures[EAU]);

    // Water parameters :
    tailleEau = 2 * rayon / GLfloat(repetition);

    // Tentative de création du motif :
    /*if (motifMNT != 0) {glDeleteLists(motifMNT, 1);}
    motifMNT = glGenLists(1);
    if (motifMNT != 0) {
        glNewList(motifMNT, GL_COMPILE);
            drawMNT();
        glEndList();
    }*/
}

void MondeViewer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(affSol) {drawSol();}
    if(affEau) {drawEau();}
    if(affCiel) {drawCiel();}
    //std::cout << glIsList(motifMNT) << std::endl;
    //if (motifMNT != 0) {glCallList(motifMNT);} else {
        if(affMNT) {if (monde.getVBO()) {drawMNT_VBO();} else {drawMNT();}}
    //}
    drawIsoligne();
    drawLight();
}

void MondeViewer::drawSol() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    textures[SOL]->bind();
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2d(0.0f,0.0f);             glVertex3f(-rayon, -rayon, hSol);
        glTexCoord2d(0.0f,repetition);       glVertex3f(-rayon, rayon, hSol);
        glTexCoord2d(repetition,repetition); glVertex3f(rayon, rayon, hSol);
        glTexCoord2d(repetition,0.0f);       glVertex3f(rayon, -rayon, hSol);
    glEnd();
    textures[SOL]->release();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

void MondeViewer::drawEau() {
    if (hEau != hSol) {
        if (!affVagues) {drawEauStatique(); return;}

        if (vagues.genererMotif()) {
            // On repete le carré :
            glPushMatrix();
            glTranslatef(-rayon - tailleEau, -rayon - tailleEau, hEau);
            for (int i = 0; i < repetition; i++) {
                glTranslatef(tailleEau, 0.0, 0.0);
                glPushMatrix();
                for (int j = 0; j < repetition; j++) {
                    glTranslatef(0.0, tailleEau, 0.0);
                    glPushMatrix();
                    glScalef(tailleEau, tailleEau, 1);
                    glCallList(vagues.getMotif());
                    glPopMatrix();
                }
                glPopMatrix();
            }
            glPopMatrix();

            vagues.libererMotif();
        } else {
            // Raté... On fait la méthode sans vagues :
            drawEauStatique();
        }
    }
}

void MondeViewer::drawEauStatique() {
    if (hEau != hSol) {
        glDisable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        textures[EAU]->bind();
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2d(0.0f,0.0f); glVertex3f(-rayon, -rayon, hEau);
            glTexCoord2d(0.0f,repetition); glVertex3f(-rayon, rayon, hEau);
            glTexCoord2d(repetition,repetition); glVertex3f(rayon, rayon, hEau);
            glTexCoord2d(repetition,0.0f); glVertex3f(rayon, -rayon, hEau);
        glEnd();
        textures[EAU]->release();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    }
}

void MondeViewer::drawCiel() {
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    textures[CIEL]->bind();
    GLUquadricObj *sphere;
    sphere = gluNewQuadric();
    gluQuadricTexture(sphere,GL_TRUE);
    gluSphere(sphere,rayon,100,100);
    gluDeleteQuadric(sphere);
    textures[CIEL]->release();
    glDisable(GL_TEXTURE_2D);
}

void MondeViewer::drawMNT_VBO() {
    if (affTexturesMNT) {
    } else {
        if (!monde.vertexBuffer.isCreated()) {std::cout << "vertexBuffer not created" << endl; return;}
        if (!monde.colorBuffer.isCreated()) {std::cout << "colorBuffer not created" << endl; return;}
        if (!monde.indiceBuffer.isCreated()) {std::cout << "indiceBuffer not created" << endl; return;}

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        monde.vertexBuffer.bind();
        monde.colorBuffer.bind();
        monde.indiceBuffer.bind();

        glVertexPointer(3, GL_FLOAT, 0, NULL);
        glColorPointer(3, GL_FLOAT, 0, NULL);
        glDrawElements(GL_TRIANGLES, monde.indiceArray.size(), GL_UNSIGNED_INT, NULL);

        monde.indiceBuffer.release();
        monde.colorBuffer.release();
        monde.vertexBuffer.release();

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

void MondeViewer::drawMNT() {
    if (affTexturesMNT) {
    } else {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, monde.vertexArray.constData());
        glColorPointer(3, GL_FLOAT, 0, monde.colorArray.constData());
        glDrawElements(GL_TRIANGLES, monde.indiceArray.size(), GL_UNSIGNED_INT, monde.indiceArray.constData());
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

void MondeViewer::drawIsoligne() {
    glColor3d(1,0,0);
    for (int i = 0; i < monde.getPolyligneArray().size(); i++) {
        Polyligne polyligne = monde.getPolyligneArray().at(i);
        glLineWidth(4);
        glBegin(GL_LINE_STRIP);
            for (int j=0; j < polyligne.getPoints()->size(); j++){
                qglviewer::Vec point = polyligne.getPoints()->at(j);
                glVertex3f(point.x, point.y, point.z);
            }
        glEnd();
    }
}

void MondeViewer::drawLight() {
    const qglviewer::Vec cameraPos = camera()->position();
    const GLfloat pos[4] = {GLfloat(cameraPos.x), GLfloat(cameraPos.y), GLfloat(cameraPos.z), 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, camera()->viewDirection());
}

bool MondeViewer::loadTexture(int side, QString src) {
    //delete textures[side];
    textures[side] = new QOpenGLTexture(QImage(QString(src)));
    return true;
}

void MondeViewer::init() {
    resetView();
}

QString MondeViewer::helpString() const {
    QString text("<h2>S i m p l e V i e w e r</h2>");
    text += "Use the mouse to move the camera around the object. ";
    text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
    text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
    text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
    text += "Simply press the function key again to restore it. Several keyFrames define a ";
    text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
    text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
    text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
    text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
    text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
    text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
    text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
    text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
    text += "Press <b>Escape</b> to exit the viewer.";
    return text;
}
