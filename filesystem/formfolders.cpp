#include "formfolders.h"
#include "network/downloadfile.h"

#include <QDebug>
#include <QStringList>


FormFolders::FormFolders(QObject *parent) :
    dir(),
    QObject(parent)
{
    QSettings settings("SimpleDrive", "General");
    access_token=settings.value("access_token").toString();

    //    QDir dir;
    dir.mkdir("/home/s/Drive/");
    dir.cd("/home/s/Drive/");

}

void FormFolders::makeRootFolder(){
    QSettings s("SimpleDrive", "Files");
    s.setIniCodec("UTF-8");
    QStringList rootFilesList = s.value("rootFiles").toStringList();
    for(auto iter = rootFilesList.begin();iter!=rootFilesList.end();iter++){
        s.beginGroup(*iter);
        QString filename =dir.absolutePath()+"/"+s.value("title").toString();
        DownloadFile *df = new DownloadFile;
        df->startDownloadFile(filename, QUrl(s.value("downloadUrl").toString()));
        s.endGroup();
    }
    QStringList rootFOldersList = s.value("foldersInRoot").toStringList();
    for(auto iter = rootFOldersList.begin();iter!=rootFOldersList.end();iter++){
        s.beginGroup(*iter);
        dir.mkdir(s.value("title").toString());
        QDir localDir(dir.absolutePath()+"/"+s.value("title").toString());
//        qWarning()<<localDir.absolutePath();
        makeOtherFolders(*iter, localDir);
        s.endGroup();
    }
    //    dir.setFilter(QDir::Dirs);
    //    QStringList entries = dir.entryList();
    //    entries.removeFirst();
    //    entries.removeFirst();
    //    for(auto iter = entries.begin();iter!=entries.end();iter++){
    //        qWarning()<<*iter;
    //    }
    //    makeOtherFolders(dir);
}

void FormFolders::makeOtherFolders(QString parentId, QDir currentDir){
    //    currentDir.setFilter(QDir::Dirs);
    //    QStringList entries = currentDir.entryList();
    //    entries.removeFirst();
    //    entries.removeFirst();
    //    for(auto iter = entries.begin();iter!=entries.end();iter++){
    //        qWarning()<<*iter;
    //    }
//    qWarning()<< currentDir.absolutePath();
    QSettings s("SimpleDrive", "Files");
    s.setIniCodec("UTF-8");
    QStringList FilesList = s.value("filesInFolders").toStringList();
    for(auto iter = FilesList.begin();iter!=FilesList.end();iter++){
        s.beginGroup(*iter);
        if(s.value("parentId").toString() == parentId){
            if(s.value("folder").toBool()){
                currentDir.mkdir(s.value("title").toString());
                QDir localDir(currentDir.absolutePath()+"/"+s.value("title").toString());
//                qWarning()<<localDir.absolutePath();
                qWarning()<<localDir.absolutePath();
                makeOtherFolders(*iter, localDir);
            } else {
                QString filename =currentDir.absolutePath()+"/"+s.value("title").toString();
                DownloadFile *df = new DownloadFile;
                df->startDownloadFile(filename, QUrl(s.value("downloadUrl").toString()));
            }
        }
        s.endGroup();
    }
}
