#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QObject>
#include <QSettings>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
//#include <QUrlQuery>
#include <QFile>

class DownloadFile : public QObject
{
    Q_OBJECT
public:
    explicit DownloadFile(QObject *parent = 0);
    
signals:
    
public slots:
        void startDownloadFile(QString filename, QUrl url);
private slots:
    void SaveFile();
    void downloadFinished();
    void slotDownloadProgress(qint64,qint64);
    
private:
    QString access_token;

    QNetworkAccessManager *pNetworkAccessManager_download_file;
    QNetworkReply *rep;
    QFile file;
};

#endif // DOWNLOADFILE_H
