#ifndef SIMPLEUPLOAD_H
#define SIMPLEUPLOAD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QMimeDatabase>
#include <QMimeType>
#include <QDebug>
#include <QSettings>

class SimpleUpload : public QObject
{
    Q_OBJECT
public:
    explicit SimpleUpload(QObject *parent = 0);
    void startUpload(QString filename);

signals:
    
public slots:

private slots:
    void getRespond(QNetworkReply *);

private:
    
     QString access_token;

    QNetworkAccessManager *pNetworkAccessManager;
};

#endif // SIMPLEUPLOAD_H
