#include "authorization.h"
#include "gui/logindialog.h"
#include "settings.h"
#include <QDebug>

Authorization::Authorization(QObject *parent):
    QObject(parent)
{
    end_point = "https://accounts.google.com/o/oauth2/auth";
    scope = "https://www.googleapis.com/auth/drive";
    client_ID = "79129249048.apps.googleusercontent.com"; //normal
    redirect_URI = "urn:ietf:wg:oauth:2.0:oob";
    response_type = "code";
    client_secret = "sX7mhNqmBjmMtQqfznn7zRwd";

    pNetworkAccessManager = new QNetworkAccessManager(this);
    pNetworkAccessManager_refresh = new QNetworkAccessManager(this);
    pNetworkAccessManager_get_email = new QNetworkAccessManager(this);
    connect(pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getToken(QNetworkReply*)));
    connect(pNetworkAccessManager_refresh, SIGNAL(finished(QNetworkReply*)), this, SLOT(getRefreshToken(QNetworkReply*)));
    connect(pNetworkAccessManager_get_email, SIGNAL(finished(QNetworkReply*)), this, SLOT(getEmail(QNetworkReply*)));

    connect(this,SIGNAL(tokenObtained()), this, SLOT(email()));
}

Authorization::~Authorization(){
}

void Authorization::Token(QString Code)
{
    QUrl url("https://accounts.google.com/o/oauth2/token");
    QNetworkRequest request;

    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QString str = "client_id=" + client_ID;
    str += "&redirect_uri=" + redirect_URI;
    str += "&client_secret=" + client_secret;
    str += "&grant_type=authorization_code";
    str += "&code=" + Code;

    QByteArray params = str.toLatin1();
    pNetworkAccessManager->post(request, params);
}


void Authorization::getToken(QNetworkReply *reply){
    QString json = reply->readAll();
    QJsonDocument sd = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject sett2 = sd.object();
    refresh_token = sett2.value(QString("refresh_token")).toString();
    access_token = sett2.value(QString("access_token")).toString();

    QSettings settings("SimpleDrive", "General");
    settings.setValue("access_token", access_token);
    settings.setValue("refresh_token", refresh_token);

    emit tokenObtained();
}

void Authorization::refreshToken(){
    QUrl url("https://accounts.google.com/o/oauth2/token");
    QNetworkRequest request;

    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QSettings settings("SimpleDrive", "General");
    refresh_token=settings.value("refresh_token").toString();

    QString str = "client_secret=" + client_secret;
    str += "&grant_type=refresh_token";
    str += "&refresh_token=" + refresh_token;
    str += "&client_id=" + client_ID;
    QByteArray params = str.toLatin1();

    pNetworkAccessManager_refresh->post(request, params);
}

void Authorization::getRefreshToken(QNetworkReply *reply){
    QString json = reply->readAll();
    QJsonDocument sd = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject sett2 = sd.object();
    access_token = sett2.value(QString("access_token")).toString();
    QSettings settings("SimpleDrive", "General");
    settings.setValue("access_token", access_token);
}

void Authorization::email(){
    QSettings settings("SimpleDrive", "General");
    access_token=settings.value("access_token").toString();
    QUrl url("https://www.googleapis.com/oauth2/v2/userinfo");
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(access_token).toLatin1());
    pNetworkAccessManager_get_email->get(request);
}

void Authorization::getEmail(QNetworkReply *reply){
    QString json = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject sett2 = jsonDoc.object();
    QSettings s("SimpleDrive", "General");
    s.setValue("email", sett2["email"].toString());

}
