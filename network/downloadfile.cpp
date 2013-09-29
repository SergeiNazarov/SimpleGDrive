#include "downloadfile.h"
#include <QDebug>
#include <QThread>

// TODO: Make progressBar working

DownloadFile::DownloadFile(QString filename, QUrl url, QProgressBar *bar, QObject *parent) :
    QObject(parent),
    progressBar(bar),
    filename(filename),
    url(url)
{
    flag_for_waiting=true;
    QSettings s("SimpleGDrive", "General");
    totalSize = s.value("totalSize").toInt();
    access_token = s.value("access_token").toString();
    pNetworkAccessManager_download_file = new QNetworkAccessManager(this);
    connect(this, SIGNAL(baton()), this, SLOT(setValue()));
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
    emit baton();
}

void DownloadFile::setValue(){
    flag_for_waiting=false;
}

void DownloadFile::slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal){
    mute.lock();
    progressBar->setMaximum(bytesTotal);
//    qint64 temp = progressBar->value() + bytesReceived;
//    emit curBytes(temp);
    progressBar->setValue(bytesReceived);
    mute.unlock();
}
