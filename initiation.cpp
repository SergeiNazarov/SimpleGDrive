#include "initiation.h"
#include <QDebug>
#include <QSettings>

Initiation::Initiation(MainWindow *mw, QObject *parent) :
    QObject(parent),
    mw(mw)
{
    connect(this, SIGNAL(AuthComplete()),mw, SLOT(show()));
    QSettings settings("SimpleDrive", "General");
    if((!settings.value("access_token").toBool())){
        LoginDialog *lg = new LoginDialog();
        lg->show();
        connect(lg,SIGNAL(TokenObtained()), mw, SLOT(show()));
        connect(lg,SIGNAL(TokenObtained()), lg, SLOT(close()));
    } else {
        emit AuthComplete();
    }
}

