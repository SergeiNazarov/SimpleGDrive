#include "formfolders.h"
#include "network/downloadfile.h"

#include <QDebug>
#include <QStringList>
#include <QTime>
#include <QCoreApplication>
#include <QThread>


FormFolders::FormFolders(DataBase *db, QStatusBar *status, QProgressBar *bar, QObject *parent) :
    QObject(parent),
    progressBar(bar),
    statusBar(status),
    db(db)
{
    QSettings settings("SimpleGDrive", "General");
    access_token=settings.value("access_token").toString();
    n=0;
    this->db->countAllFiles();
    this->db->verifyExisting();
    this->db->countDownloadedFiles();
    progressBar->setMaximum(this->db->filesQuantity);
    progressBar->setValue(this->db->downloadedFiles);
    progressBar->setTextVisible(true);

    QString text(QString("%1/%2").arg(this->db->downloadedFiles).arg(this->db->filesQuantity));
    progressBar->setFormat(text);
}

void FormFolders::makeRootFolder(QString rootDir){
    QDir dir;
    dir.cd(rootDir);

    QMap<QString, Data>::const_iterator folder = db->dataBase.constBegin();
    while (folder != db->dataBase.constEnd()) {
        if(folder.value().isFolder){
            dir.mkpath(folder.value().filename);
        }
        folder++;
    }

    QMap<QString, Data>::const_iterator file = db->dataBase.constBegin();
    while (file != db->dataBase.constEnd()) {
        if(!file.value().isFolder){
            if(file.value().isOnline){
                linkingOnlineFiles(file.value().filename, file.value().downloadUrl, file.value().title);
            } else {
                if(!file.value().exist){
                    downloadFile(file.value().filename, QUrl(file.value().downloadUrl));
                } else {
//                        qWarning()<<file.value().filename<<"exist";
                }
            }
        }
        file++;
    }

    statusBar->showMessage("Finished", 3000);

}


void FormFolders::downloadFile(QString filename, QUrl url){
    if(filename.isEmpty()) return;
    int counter=0;
    QThread t;
    qWarning()<<" "<<++n<<" "<<filename;
    DownloadFile *df = new DownloadFile(filename, url, statusBar);
    df->moveToThread(&t);
    t.start();
    connect(&t, SIGNAL(started()), df, SLOT(startDownloadFile()));
    while (df->flag_for_waiting) {
        delay(1);
        counter++;
        if(counter>10 && (!QFile(filename).exists() || QFile(filename).size()==0)){
            downloadFile(filename, url);
            t.quit();
            while(!t.isFinished()){}
            delete df;
            return;
        }
    }
    db->downloadedFiles++;
    progressBar->setValue(db->downloadedFiles);
    QString text(QString("%1/%2").arg(this->db->downloadedFiles).arg(this->db->filesQuantity));
    progressBar->setFormat(text);
    t.quit();
    while(!t.isFinished()){}

    delete df;
}

void FormFolders::delay(int n)
{
    QTime dieTime= QTime::currentTime().addSecs(n);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void FormFolders::linkingOnlineFiles(QString filename, QString url, QString title){
    QFile file;
    //    filename+=".html";  Don't know how it will work on Win and Mac or not.
    file.setFileName(filename);
    QString html;
    html=QString("<!DOCTYPE HTML>"
                 "<html lang=\"en-US\">"
                 "<head>"
                 "<meta charset=\"UTF-8\">"
                 "<meta http-equiv=\"refresh\" content=\"1;url=%1\">"
                 "<script type=\"text/javascript\">"
                 "window.location.href = \"%1\""
                 "</script>"
                 "<title>Page Redirection</title>"
                 "</head>"
                 "<body>"
                 "<!-- Note: don't tell people to `click` the link, just tell them that it is a link. -->"
                 "If you are not redirected automatically, follow the <a href='%1'>link to %2</a>"
                 "</body>"
                 "</html>").arg(url).arg(title);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << html;
    file.close();
}
