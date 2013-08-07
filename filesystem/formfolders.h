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

class FormFolders : public QObject
{
    Q_OBJECT
public:
    explicit FormFolders(QObject *parent = 0);
    void makeRootFolder(QString rootDir);

signals:

public slots:

private slots:

private:
    void makeOtherFolders(QString parentId, QDir currentDir);
    void linkingOnlineFiles(QString filename, QString url, QString title);

    QString access_token;

    QNetworkAccessManager *pNetworkAccessManager_get;
};

#endif // FORMFOLDERS_H
