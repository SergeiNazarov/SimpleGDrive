#include "downloadfile.h"
#include <QDebug>

DownloadFile::DownloadFile(QString filename, QUrl url, QStatusBar *bar, QObject *parent) :
    QObject(parent),
    statusBar(bar),
    filename(filename),
    url(url)
{
    flag_for_waiting=true;
    QSettings settings("SimpleGDrive", "General");
    totalSize = settings.value("totalSize").toInt();
    access_token = settings.value("access_token").toString();
    pNetworkAccessManager_download_file = new QNetworkAccessManager(this);
    statusBar->showMessage(QString("Downloading %1").arg(filename));
}

DownloadFile::~DownloadFile(){
    delete pNetworkAccessManager_download_file;
}

void DownloadFile::startDownloadFile(){
    file.setFileName(filename);
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(access_token).toLatin1());

    file.open(QIODevice::WriteOnly);

    reply = pNetworkAccessManager_download_file->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(SaveFile()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(slotDownloadProgress(qint64,qint64)));
}

void DownloadFile::SaveFile(){

    file.write(reply->readAll());
}

void DownloadFile::downloadFinished(){
    file.flush();
    file.close();
    flag_for_waiting=false;
}

void DownloadFile::slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal){
    if(bytesTotal>10485760){
    int temp = (double)bytesReceived/bytesTotal*100;
//    int toStatusBar = temp;
    statusBar->clearMessage();
    statusBar->showMessage(QString("Downloading %1 (%2%)").arg(filename).arg(temp));
    }
}

