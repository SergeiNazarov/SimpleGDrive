#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QObject>
#include <QSettings>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include <QProgressBar>

#include <QMutex>


class DownloadFile : public QObject
{
    Q_OBJECT
public:
    explicit DownloadFile(QString filename, QUrl url,QProgressBar *bar = 0,QObject *parent = 0);
    bool flag_for_waiting;
    QProgressBar *progressBar;
    ~DownloadFile();
signals:
    void baton();
    
public slots:
    void startDownloadFile();

private slots:
    void SaveFile();
    void downloadFinished();
    void slotDownloadProgress(qint64,qint64);
    void setValue();
    
private:
    QMutex mute;
    QString filename;
    QUrl url;
    qint64 totalSize;
    qint64 bytesBeforeDownloading;
    QString access_token;
    QNetworkAccessManager *pNetworkAccessManager_download_file;
    QNetworkReply *reply;
    QFile file;

};

#endif // DOWNLOADFILE_H
