#ifndef INITIATION_H
#define INITIATION_H

#include <QObject>
#include <QApplication>

#include "gui/mainwindow.h"
#include "gui/logindialog.h"
#include "network/authorization.h"

class Initiation : public QObject
{
    Q_OBJECT
public:
    explicit Initiation (MainWindow *mw, QObject *parent = 0);

signals:
    void AuthComplete();
    void CodeObtained();

private:
    QMainWindow *mw;
    
};

#endif // INITIATION_H
