#ifndef FORMLISTOFFILES_H
#define FORMLISTOFFILES_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class FormListOfFiles : public QObject
{
    Q_OBJECT
public:
    explicit FormListOfFiles(QObject *parent = 0);
    void formList();
    void getRootList();
    void getFullList();

signals:
    void rootListFormed(FormListOfFiles*);
    void fullListFormed(FormListOfFiles*);
    
public slots:

private slots:

    void getRootReply(QNetworkReply *);
    void getReply(QNetworkReply *);

private:

    void formRootList(QString json);

    QString access_token;
    QString jsonWithAllFiles;

    QNetworkAccessManager *pNetworkAccessManager_root;
    QNetworkAccessManager *pNetworkAccessManager;
    
};

#endif // FORMLISTOFFILES_H
