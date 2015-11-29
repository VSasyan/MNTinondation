#include "monde.h"

Monde::Monde() :
    vertexBuffer(QGLBuffer::VertexBuffer),
    indiceBuffer(QGLBuffer::IndexBuffer),
    colorBuffer(QGLBuffer::IndexBuffer)
{
    this->bornes = Bornes();
    this->pointArray = QVector<QVector3D>(0);
    this->polyligneArray = QVector<Polyligne>(0);
    this->indiceArray = QVector<GLuint>(0);
    this->colorArray = QVector<GLfloat>(0);
    this->nbX = 0;
    this->nbY = 0;
    this->VBO = false;
}

Monde::~Monde() {
    libererBuffer();
}

void Monde::setBornes(QStringList Ligne_1, QStringList Ligne_2, QStringList Ligne_n) {
    bornes.chargerLignes(Ligne_1, Ligne_2, Ligne_n);
    nbX = bornes.calnbX();
    nbY = bornes.calnbY();
}

int Monde::getNbPoints() {
    return this->pointArray.size();
}

int Monde::getNbTriangles() {
    return floor(this->indiceArray.size()/3);
}

QVector3D Monde::getPoint(int i) {
    return this->pointArray.at(i);
}

QVector<Polyligne> Monde::getPolyligneArray() {
    return this->polyligneArray;
}

bool Monde::getVBO() {
    return this->VBO;
}

QString Monde::getSizes() {
    QString txt = "";
    txt += "points : " + QString::number(sizeof(QVector3D) * pointArray.size()) + " \n";
    txt += "vertex : " + QString::number(sizeof(GLfloat) * vertexArray.size()) + " \n";
    txt += "color : " + QString::number(sizeof(GLfloat) * colorArray.size()) + " \n";
    txt += "indice : " + QString::number(sizeof(GLuint) * indiceArray.size());
    return txt;
}

Recentreur* Monde::getRecentreur() {
    return &recentreur;
}

Bornes Monde::getBornes() {
    return this->bornes;
}

void Monde::recentrer() {
    recentreur = Recentreur(bornes);
    this->bornes.recentrer(recentreur);
    this->vertexArray = QVector<GLfloat>(pointArray.size() * 3);
    for (int i = 0; i < pointArray.size(); i++) {
        pointArray[i] -= recentreur.vector;
        pointArray[i] /= recentreur.pas;
        int I = i * 3;
        vertexArray[I] = pointArray[i].x();
        vertexArray[I + 1] = pointArray[i].y();
        vertexArray[I + 2] = pointArray[i].z();
        GLfloat indice = pointArray[i].z() / bornes.max.z * 767;
        colorArray[I] = std::min(255.0f, std::max(0.0f, indice - 510))/255; // Rouge
        colorArray[I + 1] = std::min(255.0f, indice)/255; // Vert
        colorArray[I + 2] = std::min(255.0f, std::max(0.0f, indice - 255))/255; // Bleu
    }
}

QString Monde::sauverPolyligne(QString src) {
    QFile file(src);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        uint IDpoint = 0;
        stream << "id;wkt" << "\r\n";
        for (int i = 0; i < this->polyligneArray.size(); i++) {
            stream << i + 1 << ";LINESTRING(";
            for (int j = 0; j < polyligneArray[i].getSize(); j++) {
                qglviewer::Vec point = polyligneArray[i].getPoint(j, this->recentreur);
                if (j != 0) {stream << ",";}
                stream << point.x << " " << point.y << " " << point.z;
                IDpoint++;
            }
            stream << ")" << "\r\n";
        }
        return "Sauvé sous : '" + src + "'";
    } else {return "Erreur fichier.";}
}

void Monde::ajouterPoint(QStringList list) {
    this->pointArray.push_back(QVector3D(list[0].toFloat(), list[1].toFloat(), list[2].toFloat()));
    this->bornes.majZ(list[2].toFloat());
}

void Monde::trierTriangle(int i, int a, int b, int c) {
    if (pointArray[a].z() > pointArray[b].z()) {
        if (pointArray[a].z() > pointArray[c].z()) {
            if (pointArray[c].z() > pointArray[b].z()) {
                ajouterTriangle(i, b, c, a);
            } else {ajouterTriangle(i, c, b, a);}
        } else {
            ajouterTriangle(i, b, a, c);
        }
    } else {
        if (pointArray[a].z() > pointArray[c].z()) {
            ajouterTriangle(i, c, a, b);
        } else {
            if (pointArray[c].z() > pointArray[b].z()) {
                ajouterTriangle(i, a, b, c);
            } else {ajouterTriangle(i, a, c, b);}
        }
    }
}

void Monde::ajouterTriangle(int i, int x, int y, int z) {
    indiceArray[i] = x;
    indiceArray[i + 1] = y;
    indiceArray[i + 2] = z;
}

void Monde::genererBuffer() {
    int error = glGetError();
    std::cout << "entreeBuffer : " << error << std::endl;
    // Vertex buffer init
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertexArray.constData(), vertexArray.size() * sizeof(GLfloat));
    vertexBuffer.release();
    error = glGetError();
    if (!vertexBuffer.isCreated() || error != 0) {std::cout << "vertexBuffer : " << error << std::endl;}// throw 1;}

    // Indices buffer init
    indiceBuffer.create();
    indiceBuffer.bind();
    indiceBuffer.allocate(indiceArray.constData(), indiceArray.size() * sizeof(GLuint));
    indiceBuffer.release();
    error = glGetError();
    if (!indiceBuffer.isCreated() || error != 0) {std::cout << "indiceBuffer : " << error << std::endl;}// throw 2;}

    // Color buffer init
    colorBuffer.create();
    colorBuffer.bind();
    colorBuffer.allocate(colorArray.constData(), colorArray.size() * sizeof(GLfloat));
    colorBuffer.release();
    error = glGetError();
    if (!colorBuffer.isCreated() || error != 0) {std::cout << "colorBuffer : " << error << std::endl;}// throw 3;}
}

void Monde::libererBuffer() {
    if (vertexBuffer.isCreated()) {vertexBuffer.destroy();}
    if (indiceBuffer.isCreated()) {indiceBuffer.destroy();}
    if (colorBuffer.isCreated()) {colorBuffer.destroy();}
}

void Monde::setSizeIndiceArray() {
    indiceArray = QVector<GLuint>(6 * (nbX-1) * (nbY-1));
}

void Monde::setSizeColorArray() {
    colorArray = QVector<GLfloat>(3 * nbX * nbY);
}

void Monde::setVBO(bool VBO) {
    this->VBO = VBO;
    if (VBO) {
        genererBuffer();
    } else {
        libererBuffer();
    }
}

void Monde::resetPolyligne() {
    this->polyligneArray.clear();
}

void Monde::trianguler() {
    int k = 0;
    for (int i = 0; i < this->nbY - 1; i++) {
        for (int j = 0; j < this->nbX - 1; j++) {
            int n = i * nbX + j;
            this->trierTriangle(k, n, n + 1, n + nbX);
            this->trierTriangle(k + 3, n + 1, n + nbX, n + nbX + 1);
            k += 6;
        }
    }
}

void Monde::segmenter(qreal d){
    QTime t;
    t.start();
    bool efPresent = false;

    qreal Ex = 0, Ey = 0;
    qreal Fx = 0, Fy = 0;

    this->polyligneArray.clear();

    for (int i = 0; i < indiceArray.size()-2; i = i + 3) {
        int a = indiceArray[i];
        int c = indiceArray[i+2];

       if ((d >= pointArray[a].z()) || (d <= pointArray[c].z())) {

            int b = indiceArray[i+1];
            efPresent = false;

            if ((d > pointArray[a].z()) && (d < pointArray[b].z())) { // d entre a et b

                //calculer vecteur AB et AC
                qreal Xab = pointArray[b].x() - pointArray[a].x();
                qreal Yab = pointArray[b].y() - pointArray[a].y();
                qreal Zab = pointArray[b].z() - pointArray[a].z();

                qreal Xac = pointArray[c].x() - pointArray[a].x();
                qreal Yac = pointArray[c].y() - pointArray[a].y();
                qreal Zac = pointArray[c].z() - pointArray[a].z();

                //equations paramétriques d'AB et AC pour e et f les points recherchés
                qreal Et = (-pointArray[a].z() + d) / Zab;
                Ex = pointArray[a].x() + Et*Xab;
                Ey = pointArray[a].y() + Et*Yab;


                qreal Ft = (-pointArray[a].z() + d) / Zac;
                Fx = pointArray[a].x() + Ft*Xac;
                Fy = pointArray[a].y() + Ft*Yac;

                efPresent = true;

            } else if ((d < pointArray[c].z()) && (d > pointArray[b].z())) {  //d entre b et c

                //calculer vecteur CB et CA
                qreal Xcb = pointArray[b].x() - pointArray[c].x();
                qreal Ycb = pointArray[b].y() - pointArray[c].y();
                qreal Zcb = pointArray[b].z() - pointArray[c].z();

                qreal Xca = pointArray[a].x() - pointArray[c].x();
                qreal Yca = pointArray[a].y() - pointArray[c].y();
                qreal Zca = pointArray[a].z() - pointArray[c].z();

                //equations paramétriques de CB et CA pour e et f les points recherchés
                qreal Et = (-pointArray[c].z() + d) / Zcb;
                Ex = pointArray[c].x() + Et*Xcb;
                Ey = pointArray[c].y() + Et*Ycb;


                qreal Ft = (-pointArray[c].z() + d) / Zca;
                Fx = pointArray[c].x() + Ft*Xca;
                Fy = pointArray[c].y() + Ft*Yca;

                efPresent = true;

            }

            //else if ((d == pointArray[a].z()) || (d == pointArray[b].z()) || (d == pointArray[c].z())) {}

            else if ((d == pointArray[a].z()) && (d == pointArray[b].z())) {

                Ex = pointArray[a].x();
                Ey = pointArray[a].y();
                Fx = pointArray[b].x();
                Fy = pointArray[b].y();

                efPresent = true;

            } else if ((d == pointArray[b].z()) && (d == pointArray[c].z())) {

                Ex = pointArray[b].x();
                Ey = pointArray[b].y();
                Fx = pointArray[c].x();
                Fy = pointArray[c].y();

                efPresent = true;

            } else if ((d == pointArray[a].z()) && (d == pointArray[c].z())) {

                Ex = pointArray[a].x();
                Ey = pointArray[a].y();
                Fx = pointArray[c].x();
                Fy = pointArray[c].y();

                efPresent = true;

            }

            if (efPresent == true) {

                qglviewer::Vec pointE = qglviewer::Vec(Ex, Ey, d);
                qglviewer::Vec pointF = qglviewer::Vec(Fx, Fy, d);

                int jA = -1, J = polyligneArray.size();

                for (int j=0; j < J; j++){
                    if (polyligneArray[j].testerAjout(pointE, pointF)) {
                        //std::cout<<"tester ajout "<<ajoutRealisee<<std::endl;
                        if (jA == -1){
                            jA = j;
                            polyligneArray[j].ajoutPoints(pointE, pointF);
                            //std::cout<<"ajout des points à 1 polyligne, jA = "<<jA<<std::endl;
                        } else {
                            //std::cout<<"ajout des points à 2 polyligne, jA et jB = "<<jB<<" "<<jA<<std::endl;
                            polyligneArray[jA].fusionnerPoly(polyligneArray[j].getPoints());
                            polyligneArray.remove(j);
                            j = J;
                        }
                    }
                }

                if (jA == -1){
                    polyligneArray.push_back(Polyligne(pointE, pointF));
                    //std::cout<<"création nouvelle polyligne : "<<jB<<" "<<jA<<" "<<std::endl;
                }
            }
        }
       //std::cout<<"nombre polyligne "<<polyligneArray.size()<<std::endl;
    }
    std::cout << "Temps segmentation : " << t.elapsed() << "ms (" << polyligneArray.size() << " polylignes)" << std::endl;
}

