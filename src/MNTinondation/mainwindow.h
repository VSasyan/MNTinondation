#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QString>
#include <QMainWindow>
#include "mondeviewer.h"

#include "monde.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MondeViewer *viewer;

    void afficherTitre();
    void chargerFichier(QString src);

    int tempsChargement;

private slots:
    void ouvrirFichier();
    void lancerAnimation();
    void stopperAnimation();
    void resetAnimation();
    void calculerIsoligne();
    void effacerIsoligne();
    void sauverIsoligne();
    void switchSol();
    void switchEau();
    void switchVagues();
    void switchMNT();
    void switchCiel();
    void switchVBO();
    void switchTextures();
    void validerParametres();
    void chargerFichierOK(int t);
    void chargerFichierFini();
    void setMessageStatutBarre(QString txt);
};

#endif // MAINWINDOW_H
