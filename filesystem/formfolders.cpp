#include "formfolders.h"
#include "network/downloadfile.h"

#include <QDebug>
#include <QStringList>
#include <QTime>
#include <QCoreApplication>
#include <QThread>


FormFolders::FormFolders(QStatusBar *status, QProgressBar *bar, QObject *parent) :
    QObject(parent),
    progressBar(bar),
    statusBar(status)
{
    QSettings settings("SimpleDrive", "General");
    access_token=settings.value("access_token").toString();
    numberOfFiles=0;
    n=0;
    countTotalDriveSpace();
}

void FormFolders::makeRootFolder(QString rootDir){
    QDir dir;
    dir.cd(rootDir);

    QSettings s("SimpleDrive", "Files");
    s.setIniCodec("UTF-8");
    QStringList rootFilesList = s.value("Files").toStringList();
    for(auto iter = rootFilesList.begin();iter!=rootFilesList.end();iter++){
        s.beginGroup(*iter);
        QString filename =dir.absolutePath()+"/"+s.value("title").toString();
        if(s.value("online").toBool()){
            linkingOnlineFiles(filename, s.value("downloadUrl").toString(), s.value("title").toString());
        } else {
            downloadFile(filename, QUrl(s.value("downloadUrl").toString()));
            qWarning()<<"downloading"<<filename;
        }
        s.endGroup();
    }
    QStringList rootFoldersList = s.value("Folders").toStringList();
    for(auto iter = rootFoldersList.begin();iter!=rootFoldersList.end();iter++){
        s.beginGroup(*iter);
        dir.mkdir(s.value("title").toString());
        QDir localDir(dir.absolutePath()+"/"+s.value("title").toString());
        makeOtherFolders(*iter, localDir);
        s.endGroup();
    }
    statusBar->showMessage("Finished", 3000);

}

void FormFolders::makeOtherFolders(QString parentId, QDir currentDir){
    QSettings s("SimpleDrive", "Files");
    s.setIniCodec("UTF-8");
    QStringList FilesList = s.value("filesInFolders").toStringList();
    for(auto iter = FilesList.begin();iter!=FilesList.end();iter++){
        s.beginGroup(*iter);
        if(s.value("parentId").toString() == parentId){
            if(s.value("folder").toBool()){
                currentDir.mkdir(s.value("title").toString());
                QDir localDir(currentDir.absolutePath()+"/"+s.value("title").toString());
                makeOtherFolders(*iter, localDir);
            } else {
                QString filename = currentDir.absolutePath()+"/"+s.value("title").toString();
                if(s.value("online").toBool()){
                    linkingOnlineFiles(filename, s.value("downloadUrl").toString(), s.value("title").toString());
                } else {
                    if(!QFile(filename).exists()){
                        downloadFile(filename, QUrl(s.value("downloadUrl").toString()));
                    } else {
//                        qWarning()<<filename<<"exist";
                    }
                }
            }
        }
        s.endGroup();
    }
}

void FormFolders::downloadFile(QString filename, QUrl url){
    int counter=0;
    statusBar->showMessage(QString("Downloading "+filename));
    QThread t;
    qWarning()<<" "<<++n<<" "<<filename;
    DownloadFile *df = new DownloadFile(filename, url, progressBar);
    df->moveToThread(&t);
    t.start();
    connect(&t, SIGNAL(started()), df, SLOT(startDownloadFile()));
    while (df->flag_for_waiting) {
        delay(1);
        counter++;
        if(counter>10 && !QFile(filename).exists()){
            downloadFile(filename, url);
            break;
        }
    }
    t.quit();
    while(!t.isFinished()){}

    delete df;
}

void FormFolders::counter(){
    --numberOfFiles;
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


void FormFolders::countTotalDriveSpace(){
    QSettings s("SimpleDrive", "Files");
    s.setIniCodec("UTF-8");
    int totalSize=0;
    QStringList FilesInFOldersList = s.value("filesInFolders").toStringList();
    for(auto iter = FilesInFOldersList.begin();iter!=FilesInFOldersList.end();iter++){
        s.beginGroup(*iter);
        totalSize+=s.value("fileSize").toInt();
        s.endGroup();
    }
    QStringList FilesList = s.value("Files").toStringList();
    for(auto iter = FilesList.begin();iter!=FilesList.end();iter++){
        s.beginGroup(*iter);
        totalSize+=s.value("fileSize").toInt();
        s.endGroup();
    }
    QSettings generalSettings("SimpleDrive", "General");
    generalSettings.setValue("totalSize", totalSize);
}
