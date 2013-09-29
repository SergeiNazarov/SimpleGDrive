#include "multipartupload.h"

MultipartUpload::MultipartUpload(QObject *parent) :
    QObject(parent)
{
    QSettings settings("SimpleDrive", "General");
    access_token=settings.value("access_token").toString();

    pNetworkAccessManager = new QNetworkAccessManager(this);
    connect(pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getRespond(QNetworkReply*)));

}

void MultipartUpload::startUpload(QString filename) {
    QUrl url("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart");
    QNetworkRequest request;

    QFile file(filename);
    QFileInfo fileInfo(file);
    if (!file.open(QIODevice::ReadOnly))
            return;

    QString parentId = "root";
    QString folder = filename.replace(fileInfo.fileName(), "");
    folder.chop(1);
    QSettings s("SimpleDrive", "Files");
    QStringList allFiles = s.value("rootFiles").toStringList() + s.value("filesInFolders").toStringList();
    for(auto iter = allFiles.begin();iter!=allFiles.end();iter++){
        s.beginGroup(*iter);
        if(s.value("filename").toString() == folder){
            parentId=*iter;
            break;
        }
        s.endGroup();
    }

    QString boundary = "foo_bar_baz";

    QString metadata = QString("{"
                               "\"title\": \"%1\","
                               "\"parents\": ["
                               "{ \"id\": \"%2\"}"
                               "]"
                               "}").arg(fileInfo.fileName()).arg(parentId);

    QMimeDatabase mimeData;
    QMimeType mimeType = mimeData.mimeTypeForFile(filename);

    request.setUrl(url);
    request.setRawHeader("Content-Type", QString("multipart/related; boundary=\"%1\"").arg(boundary).toLatin1());            //mimeType.name().toLatin1());
    request.setRawHeader("Authorization", QString("Bearer %1").arg(access_token).toLatin1());

    QByteArray data = QString("--" + boundary + "\n").toLatin1();
    data += QString("Content-Type: application/json; charset=UTF-8\n\n").toLatin1();
    data += QString(metadata + "\n\n").toLatin1();
    data += QString("--" + boundary + "\n").toLatin1();
    data += QString("Content-Type: " + mimeType.name() + "\n\n").toLatin1();
    data += file.readAll();
    data += QString("\n--" + boundary + "--").toLatin1();
    file.close();


    request.setRawHeader("Content-Length", QString::number(data.size()).toLatin1());
    QNetworkReply *reply = pNetworkAccessManager->post(request, data);
    connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadProgress(qint64,qint64)));
}

void MultipartUpload::uploadProgress(qint64 all, qint64 current){
    qWarning()<<current<<"/"<<all;
}

void MultipartUpload::getRespond(QNetworkReply *reply){
    QString respond = reply->readAll();
    qWarning()<<respond;
}
