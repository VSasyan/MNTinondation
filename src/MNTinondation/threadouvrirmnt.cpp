#include "threadouvrirmnt.h"

ThreadOuvrirMNT::ThreadOuvrirMNT(Monde *monde, QString &src) :
    monde(monde)
{
    setParent(0);
    moveToThread(this);
    this->src = src;
}

void ThreadOuvrirMNT::run() {
    t.start();
    emit sendText(QString("Ouverture du MNT..."));
    QFile fichier(src);
    if (fichier.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit sendText(QString("Ouverture du MNT ('" + src + "') : Lecture des points..."));
        QTextStream in(&fichier);
        QStringList Ligne_1 = in.readLine().split(" ");
        monde->ajouterPoint(Ligne_1);
        QStringList Ligne_2 = in.readLine().split(" ");
        monde->ajouterPoint(Ligne_2);
        QStringList Ligne_n;
        while (!in.atEnd()) {
             Ligne_n = in.readLine().split(" ");
             monde->ajouterPoint(Ligne_n);
        }

        emit sendText(QString("Préparation du MNT..."));
        monde->setBornes(Ligne_1, Ligne_2, Ligne_n);
        monde->setSizeIndiceArray();
        monde->setSizeColorArray();

        emit sendText(QString("Préparation du MNT : Triangulation..."));
        monde->trianguler();

        emit sendText(QString("Préparation du MNT : Recentrage..."));
        monde->recentrer();

        emit sendText(QString("Opération terminée !"));
        emit result(t.elapsed());
        emit terminated();
    } else {emit result(-1000);}

    //this->terminate();
    //this->wait();
}
