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

class FormFolders : public QObject
{
    Q_OBJECT
public:
    explicit FormFolders(QStatusBar *status=0 ,QProgressBar *bar=0, QObject *parent = 0);
    void makeRootFolder(QString rootDir);

signals:

public slots:

private slots:
    void counter();

private:
    void delay(int n);
    void makeOtherFolders(QString parentId, QDir currentDir);
    void linkingOnlineFiles(QString filename, QString url, QString title);
    void downloadFile(QString filename, QUrl url);
    void countTotalDriveSpace();

    int numberOfFiles;
    int n;

    QProgressBar *progressBar;
    QStatusBar *statusBar;

    QString access_token;

    QNetworkAccessManager *pNetworkAccessManager_get;
};

#endif // FORMFOLDERS_H
