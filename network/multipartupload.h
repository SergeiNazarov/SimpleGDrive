#ifndef MULTIPARTUPLOAD_H
#define MULTIPARTUPLOAD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QMimeType>
#include <QDebug>
#include <QSettings>

class MultipartUpload : public QObject
{
    Q_OBJECT
public:
    explicit MultipartUpload(QObject *parent = 0);
    void startUpload(QString filename);

signals:

public slots:

private slots:
    void getRespond(QNetworkReply *);
    void uploadProgress(qint64, qint64);

private:

    QString access_token;

    QNetworkAccessManager *pNetworkAccessManager;
};

#endif // MULTIPARTUPLOAD_H
