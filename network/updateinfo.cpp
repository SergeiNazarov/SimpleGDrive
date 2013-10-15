#include "updateinfo.h"

//This class wasn't tested.

UpdateInfo::UpdateInfo(QObject *parent) :
    QObject(parent)
{
    QSettings settings("SimpleGDrive", "General");
    access_token=settings.value("access_token").toString();

    networkAccessManager = new QNetworkAccessManager(this);
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getRespond(QNetworkReply*)));
}

void UpdateInfo::startUpdateInfo(QString fileId, QString gDate){
    QUrl url(QString("https://www.googleapis.com/drive/v2/files/%1?setModifiedDate=true").arg(fileId));
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(access_token).toLatin1());

    QByteArray data = QString("{\"modifiedDate\": \"%1\"}").arg(gDate).toLatin1();

    networkAccessManager->put(request, data);
}

void UpdateInfo::getRespond(QNetworkReply *reply){
    QString respond = reply->readAll();
}
