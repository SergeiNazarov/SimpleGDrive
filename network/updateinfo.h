#ifndef UPDATEINFO_H
#define UPDATEINFO_H

#include <QObject>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class UpdateInfo : public QObject
{
    Q_OBJECT
public:
    explicit UpdateInfo(QObject *parent = 0);
    void startUpdateInfo(QString fileId, QString gDate);
    
signals:
    
public slots:
    void getRespond(QNetworkReply *);

private:
    QNetworkAccessManager *networkAccessManager;

    QString access_token;
    
};

#endif // UPDATEINFO_H
