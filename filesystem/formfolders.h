#ifndef FORMFOLDERS_H
#define FORMFOLDERS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QProgressBar>
#include <QStatusBar>

#include "data.h"
#include "database.h"

class FormFolders : public QObject
{
    Q_OBJECT
public:
    explicit FormFolders(DataBase *db, QStatusBar *status=0 ,QProgressBar *bar=0, QObject *parent = 0);
    void makeRootFolder(QString rootDir);

signals:

public slots:

private slots:

private:
    void delay(int n);
    void makeOtherFolders(QString parentId, QDir currentDir);
    void linkingOnlineFiles(QString filename, QString url, QString title);
    void downloadFile(QString filename, QUrl url);

    QProgressBar *progressBar;
    QStatusBar *statusBar;

    QString access_token;

    QNetworkAccessManager *pNetworkAccessManager_get;

    DataBase *db;
};

#endif // FORMFOLDERS_H
