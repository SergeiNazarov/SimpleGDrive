#include "formfolders.h"
#include "network/downloadfile.h"

#include <QDebug>
#include <QStringList>


FormFolders::FormFolders(QObject *parent) :
    QObject(parent)
{
    QSettings settings("SimpleDrive", "General");
    access_token=settings.value("access_token").toString();
}

void FormFolders::makeRootFolder(QString rootDir){
    QDir dir;
    dir.cd(rootDir);

    QSettings s("SimpleDrive", "Files");
    s.setIniCodec("UTF-8");
    QStringList rootFilesList = s.value("rootFiles").toStringList();
    for(auto iter = rootFilesList.begin();iter!=rootFilesList.end();iter++){
        s.beginGroup(*iter);
        QString filename =dir.absolutePath()+"/"+s.value("title").toString();
        if(s.value("online").toBool()){
            linkingOnlineFiles(filename, s.value("downloadUrl").toString(), s.value("title").toString());
        } else {
        DownloadFile *df = new DownloadFile;
        df->startDownloadFile(filename, QUrl(s.value("downloadUrl").toString()));
        }
        s.endGroup();
    }
    QStringList rootFOldersList = s.value("foldersInRoot").toStringList();
    for(auto iter = rootFOldersList.begin();iter!=rootFOldersList.end();iter++){
        s.beginGroup(*iter);
        dir.mkdir(s.value("title").toString());
        QDir localDir(dir.absolutePath()+"/"+s.value("title").toString());
        makeOtherFolders(*iter, localDir);
        s.endGroup();
    }

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
                QString filename =currentDir.absolutePath()+"/"+s.value("title").toString();
                if(s.value("online").toBool()){
                    linkingOnlineFiles(filename, s.value("downloadUrl").toString(), s.value("title").toString());
                } else {
                DownloadFile *df = new DownloadFile;
                df->startDownloadFile(filename, QUrl(s.value("downloadUrl").toString()));
                }
            }
        }
        s.endGroup();
    }
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
