#include "simpleupload.h"

SimpleUpload::SimpleUpload(QObject *parent) :
    QObject(parent)
{
    QSettings settings("SimpleGDrive", "General");
    access_token=settings.value("access_token").toString();

    pNetworkAccessManager = new QNetworkAccessManager(this);
    connect(pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getRespond(QNetworkReply*)));

}

void SimpleUpload::startUpload(QString filename) {
    QUrl url("https://www.googleapis.com/upload/drive/v2/files?uploadType=media");
    QNetworkRequest request;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
            return;


    QMimeDatabase mimeData;
    QMimeType mimeType = mimeData.mimeTypeForFile(filename);


    request.setUrl(url);
    request.setRawHeader("Content-Type", mimeType.name().toLatin1());
    request.setRawHeader("Content-Length", QString::number(file.size()).toLatin1());
    request.setRawHeader("Authorization", QString("Bearer %1").arg(access_token).toLatin1());

    QByteArray data = file.readAll();
    file.close();

    pNetworkAccessManager->post(request, data);
}

void SimpleUpload::getRespond(QNetworkReply *reply){
    QString respond = reply->readAll();
    qWarning()<<respond;
}
