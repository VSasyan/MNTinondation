#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QFileDialog>
#include <QProgressBar>

#include "threadouvrirmnt.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    viewer = ui->widget;

    QObject::connect(ui->actionOuvrir, SIGNAL(triggered()), this, SLOT(ouvrirFichier()));
    QObject::connect(ui->actionInondation, SIGNAL(triggered()), this, SLOT(lancerAnimation()));
    QObject::connect(ui->actionPause, SIGNAL(triggered()), this, SLOT(stopperAnimation()));
    QObject::connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(resetAnimation()));
    QObject::connect(ui->actionIsoligne, SIGNAL(triggered()), this, SLOT(calculerIsoligne()));
    QObject::connect(ui->actionEffacer, SIGNAL(triggered()), this, SLOT(effacerIsoligne()));
    QObject::connect(ui->actionSauver, SIGNAL(triggered()), this, SLOT(sauverIsoligne()));
    QObject::connect(ui->actionSol, SIGNAL(triggered()), this, SLOT(switchSol()));
    QObject::connect(ui->actionEau, SIGNAL(triggered()), this, SLOT(switchEau()));
    QObject::connect(ui->actionVagues, SIGNAL(triggered()), this, SLOT(switchVagues()));
    QObject::connect(ui->actionMNT, SIGNAL(triggered()), this, SLOT(switchMNT()));
    QObject::connect(ui->actionCiel, SIGNAL(triggered()), this, SLOT(switchCiel()));
    QObject::connect(ui->actionVBO, SIGNAL(triggered()), this, SLOT(switchVBO()));
    QObject::connect(ui->actionCouleurs_Textures, SIGNAL(triggered()), this, SLOT(switchTextures()));
    QObject::connect(ui->valider, SIGNAL(clicked()), this, SLOT(validerParametres()));

    ui->vitesse->setText(QString::number(viewer->getVitesse()));
    ui->precision->setText(QString::number(viewer->getPrecision()));
    QRegExp rx("(|\"|/|\\.|[0-9]){30}");
    ui->vitesse->setValidator(new QRegExpValidator(rx, this));
    ui->precision->setValidator(new QRegExpValidator(rx, this));

    viewer->show();

    QFile file("../../data/double.xyz");
    if (file.exists()) {chargerFichier("../../data/double.xyz");}
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::ouvrirFichier() {
    QString src = QFileDialog::getOpenFileName(this, tr("Open File"), "../../data", tr("MNT (*.xyz)"));
    if (src != "") {
        ui->statusBar->showMessage("Ouverture de '" + src + "'", 2500);
        chargerFichier(src);
    } else {
        ui->statusBar->showMessage("Annulation...", 2500);
    }
}

void MainWindow::chargerFichier(QString src) {
    ui->actionOuvrir->setEnabled(false);
    ui->actionInondation->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionReset->setEnabled(false);
    ui->actionIsoligne->setEnabled(false);
    ui->vitesse->setEnabled(false);
    ui->precision->setEnabled(false);
    ui->valider->setEnabled(false);
    ui->widget->setEnabled(false);

    viewer->stopAnimation();
    viewer->monde = Monde();
    ThreadOuvrirMNT *thread = new ThreadOuvrirMNT(&viewer->monde, src);
    connect(thread, SIGNAL(sendText(QString)), this, SLOT(setMessageStatutBarre(QString)));
    connect(thread, SIGNAL(result(int)), this, SLOT(chargerFichierOK(int)));
    connect(thread, SIGNAL(terminated()), this, SLOT(chargerFichierFini()));
    thread->start();
}

void MainWindow::chargerFichierOK(int t) {
    tempsChargement = t/1000;
    afficherTitre();
    ui->actionOuvrir->setEnabled(true);
    std::cout << viewer->monde.getSizes().toStdString() << std::endl;
}

void MainWindow::chargerFichierFini() {
    viewer->resetView();
    ui->actionInondation->setEnabled(true);
    ui->actionPause->setVisible(false);
    ui->actionPause->setEnabled(true);
    ui->actionReset->setEnabled(true);
    ui->actionIsoligne->setEnabled(true);
    ui->vitesse->setEnabled(true);
    ui->precision->setEnabled(true);
    ui->valider->setEnabled(true);
    ui->widget->setEnabled(true);
    viewer->startAnimation();
    if (this->viewer->monde.getVBO()) {viewer->monde.genererBuffer();}
}

void MainWindow::setMessageStatutBarre(QString txt) {
    ui->statusBar->showMessage(txt);
}

void MainWindow::afficherTitre() {
    QString txt;
    if (tempsChargement != -1) {
        txt = "MNTinondation (" + QString::number(tempsChargement) + " s) : " +
                    QString::number(viewer->monde.getNbPoints()) + " points ; " +
                    QString::number(viewer->monde.getNbTriangles()) + " triangles ; " +
                    "zMin = " + QString::number(viewer->monde.getBornes().getMin().z) + " ; " +
                    "zMax = " + QString::number(viewer->monde.getBornes().getMax().z);
    } else {
        txt = "Erreur à l'ouverture du fichier !";
    }
    ui->statusBar->showMessage(txt, 2500);
    setWindowTitle(txt);
}

void MainWindow::lancerAnimation() {
    viewer->lancerAnimation();
    ui->actionInondation->setVisible(false);
    ui->actionPause->setVisible(true);
    ui->actionIsoligne->setEnabled(false);
}

void MainWindow::stopperAnimation() {
    viewer->stopperAnimation();
    ui->actionInondation->setVisible(true);
    ui->actionPause->setVisible(false);
    ui->actionIsoligne->setEnabled(true);
}

void MainWindow::resetAnimation() {
    viewer->resetAnimation();
}

void MainWindow::calculerIsoligne() {
    viewer->segmenter();
    ui->actionEffacer->setVisible(true);
    ui->actionSauver->setVisible(true);
}

void MainWindow::effacerIsoligne() {
    viewer->monde.resetPolyligne();
    ui->actionEffacer->setVisible(false);
    ui->actionSauver->setVisible(false);
}

void MainWindow::sauverIsoligne() {
    float h = viewer->getHEau();
    h *= viewer->monde.getRecentreur()->pas;
    h += viewer->monde.getRecentreur()->delta.z;
    QString title = "Save File (h = " + QString::number(h) + ")";
    QString src = QFileDialog::getSaveFileName(this, tr(title.toStdString().c_str()), "../../data", tr("CSV (*.csv)"));
    if (src != "") {
        ui->statusBar->showMessage("Sauvegarde sous '" + src + "'", 2500);
        ui->statusBar->showMessage(viewer->monde.sauverPolyligne(src), 2500);
    } else {
        ui->statusBar->showMessage("Annulation...", 2500);
    }
}

void MainWindow::switchSol() {
    viewer->setAffSol(!viewer->getAffSol());
    ui->actionSol->setChecked(viewer->getAffSol());
}

void MainWindow::switchEau() {
    viewer->setAffEau(!viewer->getAffEau());
    ui->actionEau->setChecked(viewer->getAffEau());
}

void MainWindow::switchVagues() {
    viewer->setAffVagues(!viewer->getAffVagues());
    ui->actionVagues->setChecked(viewer->getAffVagues());
}

void MainWindow::switchMNT() {
    viewer->setAffMNT(!viewer->getAffMNT());
    ui->actionMNT->setChecked(viewer->getAffMNT());
}

void MainWindow::switchCiel() {
    viewer->setAffCiel(!viewer->getAffCiel());
    ui->actionCiel->setChecked(viewer->getAffCiel());
}

void MainWindow::switchTextures() {
    viewer->stopAnimation();
    viewer->setAffTexturesMNT(!viewer->getAffTexturesMNT());
    ui->actionCouleurs_Textures->setChecked(viewer->getAffTexturesMNT());
    viewer->startAnimation();
}

void MainWindow::switchVBO() {
    viewer->stopAnimation();
    viewer->monde.setVBO(!viewer->monde.getVBO());
    ui->actionVBO->setChecked(viewer->monde.getVBO());
    viewer->startAnimation();
}

void MainWindow::validerParametres() {
    viewer->setVitesse(ui->vitesse->text().toFloat());
    viewer->setPrecision(ui->precision->text().toFloat());
    QString txt = "Vitesse (" + QString::number(viewer->getVitesse()) + " m/s) et " +
            "Précision (" + QString::number(viewer->getPrecision()) + " m) bien mis à jour. ";
    ui->statusBar->showMessage(txt, 2500);
}
