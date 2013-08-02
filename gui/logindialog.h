#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QWebFrame>
#include "network/authorization.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

signals:
    void CodeObtained();
    void TokenObtained();
private slots:
    void obtainCode();


private:
    Ui::LoginDialog *ui;

    int count;
    QString Code;
    QString EndPoint;
    QString Scope;
    QString ClientID;
    QString RedirectURI;
    QString ResponseType;
};

#endif // LOGINDIALOG_H
