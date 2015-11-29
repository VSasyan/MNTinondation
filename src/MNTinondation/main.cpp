#include "mainwindow.h"
#include "monde.h"
#include "mondeviewer.h"

#include <QTime>
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
