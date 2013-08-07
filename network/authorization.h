#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QSettings>
#include <QDebug>

#include "gui/logindialog.h"

class Authorization: public QObject
{
    Q_OBJECT
public:
    explicit Authorization(QObject *parent = 0);
    ~Authorization();
    void Token(QString code);
    void refreshToken();

public slots:
    void email();

private slots:
    void getToken(QNetworkReply *);
    void getRefreshToken(QNetworkReply *);
    void getEmail(QNetworkReply *);

signals:
    void tokenObtained();

private:
    QString end_point;
    QString scope;
    QString client_ID;
    QString redirect_URI;
    QString response_type;
    QString client_secret;
    QString code;
    QString access_token;
    QString refresh_token;

    QNetworkAccessManager *pNetworkAccessManager;
    QNetworkAccessManager *pNetworkAccessManager_get_email;
    QNetworkAccessManager *pNetworkAccessManager_refresh;

};

#endif // AUTHORIZATION_H
