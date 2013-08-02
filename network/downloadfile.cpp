#include "downloadfile.h"

DownloadFile::DownloadFile(QObject *parent) :
    QObject(parent)
{
    QSettings s("SimpleDrive", "General");
    access_token = s.value("access_token").toString();
    pNetworkAccessManager_download_file = new QNetworkAccessManager(this);
}

void DownloadFile::startDownloadFile(QString filename, QUrl url){
    file.setFileName(filename);
//    QUrl url(ui->lineEdit_2->text());
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(access_token).toLatin1());

    file.open(QIODevice::WriteOnly);

    rep = pNetworkAccessManager_download_file->get(request);

    connect(rep, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(rep, SIGNAL(readyRead()), this, SLOT(SaveFile()));
    connect(rep, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(slotDownloadProgress(qint64,qint64)));
}

void DownloadFile::SaveFile(){

    file.write(rep->readAll());
//    ui->textBrowser->setText(rep->readAll());
//    ui->progressBar->show();
}

void DownloadFile::downloadFinished(){
    file.flush();
    file.close();
//    ui->progressBar->hide();
}

void DownloadFile::slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal){

//    ui->progressBar->setMaximum(bytesTotal);
//    ui->progressBar->setValue(bytesReceived);

}
