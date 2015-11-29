#ifndef THREADOUVRIRMNT_H
#define THREADOUVRIRMNT_H

#include <QThread>
#include <QString>
#include <QTime>

#include <monde.h>

class ThreadOuvrirMNT : public QThread {
    Q_OBJECT
public:
    ThreadOuvrirMNT(Monde *monde, QString &src);

protected:
    void run();
    Monde *monde;
    QString src;
    QTime t;

signals:
    void sendText(QString);
    void result(int);
    void terminated();
};

#endif // THREADOUVRIRMNT_H
