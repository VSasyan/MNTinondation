#ifndef THREADOUVRIRMNT_H
#define THREADOUVRIRMNT_H

#include <QThread>
#include <QString>
#include <QTime>

#include <monde.h>
#include <mainwindow.h>

class ThreadOuvrirMNT : public QThread {
    Q_OBJECT
public:
    ThreadOuvrirMNT(Monde *monde, MainWindow *mainwindow, QString &src);

protected:
    void run();
    Monde *monde;
    MainWindow *mainwindow;
    QString src;
    QTime t;

signals:
    void sendText(QString);
    void result(int);
    void terminated();
};

#endif // THREADOUVRIRMNT_H
