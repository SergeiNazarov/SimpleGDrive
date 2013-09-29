#include "formlistoffiles.h"
#include "formfolders.h"
#include <QStringList>
#include <QDebug>


FormListOfFiles::FormListOfFiles(QTreeWidget *tw, QObject *parent) :
    QObject(parent),
    treeWidget(tw)
{
    treeWidget->clear();
    QSettings settings("SimpleGDrive", "General");
    access_token=settings.value("access_token").toString();
    networkAccessManager = new QNetworkAccessManager(this);
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getReply(QNetworkReply*)));
    isRoot=true;
}

void FormListOfFiles::startObtaining(){
    getList("root");
}



void FormListOfFiles::getList(QString folderId){
    QUrl url(QString("https://www.googleapis.com/drive/v2/files?maxResults=10000&q='%1' in parents").arg(folderId));
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(access_token).toLatin1());
    networkAccessManager->get(request);
}


void FormListOfFiles::getReply(QNetworkReply *reply){
    QString json = reply->readAll();
    if(isRoot){
        formRootList(json);
        isRoot=false;
    } else {
        mutex.lock();
        bigJson.push_back(json);
        mutex.unlock();
        formList(json);
    }
}

void FormListOfFiles::formRootList(QString json){
    Settings = new QSettings("SimpleGDrive", "Files");
    Settings->setIniCodec("UTF-8");
    Settings->clear();
    QStringList FoldersList, FilesList;
    QSettings generalSettings("SimpleGDrive", "General");
    QString rootPath = generalSettings.value("rootDir").toString();
    //        Settings->setIniCodec("UTF-8");
    QJsonDocument jd = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject globalObject = jd.object();
    QJsonArray arrayWithItems(globalObject["items"].toArray());
    for(auto iter = arrayWithItems.begin();iter!=arrayWithItems.end();  ++iter)
    {
        QJsonObject file = (*iter).toObject();
        QJsonObject labels_object = file["labels"].toObject();

        if(!labels_object["trashed"].toBool()){
            Settings->beginGroup(file["id"].toString());
            Settings->setValue("Path", rootPath);
            Settings->setValue("title", file["title"].toString());
            Settings->setValue("modifiedDate", file["modifiedDate"].toString());
            Settings->setValue("icon", file["iconLink"].toString().mid(44));
            QJsonArray parents_array(file["parents"].toArray());
            QJsonObject parent_object = parents_array.first().toObject();
            Settings->setValue("parentId", parent_object["id"].toString());
            if(file["mimeType"].toString() == "application/vnd.google-apps.folder"){
                Settings->setValue("folder",true);
                FoldersList.push_back(file["id"].toString());
                //                qWarning()<<file["title"].toString();
                getList(file["id"].toString());
            } else {
                FilesList.push_back(file["id"].toString());
                if(file["mimeType"].toString().contains("google")){
                    Settings->setValue("downloadUrl", file["alternateLink"].toString());
                    Settings->setValue("online", true);
                } else {
                    Settings->setValue("downloadUrl", file["downloadUrl"].toString());
                }
                Settings->setValue("originalFilename", file["originalFilename"].toString());
                Settings->setValue("fileExtension", file["fileExtension"].toString());
                Settings->setValue("md5Checksum", file["md5Checksum"].toString());
                Settings->setValue("fileSize", file["fileSize"].toString());
            }
            Settings->endGroup();
            addRootQTreeWidgetItem(file["id"].toString(), Settings, rootPath);
        }
    }

    if(!FoldersList.empty()) Settings->setValue("Folders", FoldersList);
    if(!FilesList.empty()) Settings->setValue("Files", FilesList);

    generalSettings.setValue("listFormed", true);
}


void FormListOfFiles::formList(QString json){
    QSettings s("SimpleGDrive", "Files");
    s.setIniCodec("UTF-8");
    QJsonDocument jd = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject globalObject = jd.object();
    QJsonArray arrayWithItems(globalObject["items"].toArray());
    for(auto iter = arrayWithItems.begin();iter!=arrayWithItems.end();  ++iter)
    {
        QJsonObject file = (*iter).toObject();
        QJsonObject labels_object = file["labels"].toObject();

        if(!labels_object["trashed"].toBool()){
            s.beginGroup(file["id"].toString());
            mutex.lock();
            filesInFolders.push_back(file["id"].toString());
            mutex.unlock();
            s.setValue("download", true);
            s.setValue("title", file["title"].toString());
            s.setValue("modifiedDate", file["modifiedDate"].toString());
            s.setValue("icon", file["iconLink"].toString().mid(44));
            QJsonArray parents_array(file["parents"].toArray());
            QJsonObject parent_object = parents_array.first().toObject();
            s.setValue("parentId", parent_object["id"].toString());
            if(file["mimeType"].toString() == "application/vnd.google-apps.folder"){
                s.setValue("folder",true);
                getList(file["id"].toString());
            } else {
                if(file["mimeType"].toString().contains("google")){
                    s.setValue("downloadUrl", file["alternateLink"].toString());
                    s.setValue("online", true);
                } else {
                    s.setValue("downloadUrl", file["downloadUrl"].toString());
                }
                s.setValue("originalFilename", file["originalFilename"].toString());
                s.setValue("fileExtension", file["fileExtension"].toString());
                s.setValue("md5Checksum", file["md5Checksum"].toString());
                s.setValue("fileSize", file["fileSize"].toString());
            }
            s.endGroup();
        }
    }

    mutex.lock();
    if(!filesInFolders.isEmpty()) s.setValue("filesInFolders", filesInFolders);
    mutex.unlock();
}

void FormListOfFiles::addRootQTreeWidgetItem(QString id, QSettings *s, QString path){
    QString folderPath;
    s->beginGroup(id);
    bool is_folder = s->value("folder").toBool();
    QString title = s->value("title").toString();
    if(is_folder){
        folderPath = path + "/" + title;
    }

    s->setValue("filename", QString(path + "/" + title));

    QIcon icon;
    QFile file(QString(":/fileIcons/icons/%1").arg(s->value("icon").toString()));
    if(file.exists()){
        icon = QIcon(QString(":/fileIcons/icons/%1").arg(s->value("icon").toString()));
    } else {
        icon = QIcon(QString(":/fileIcons/icons/icon_10_generic_list.png"));
    }
    s->endGroup();
    QTreeWidgetItem *itm = new QTreeWidgetItem(treeWidget);
    itm->setText(0, title);
    itm->setIcon(0, icon);
    treeWidget->addTopLevelItem(itm);
    QStringList filesInFolders = s->value("filesInFolders").toStringList();
    if(is_folder){
        addChildQTreeWidgetItem(itm,id,s,filesInFolders, folderPath);
    }
}

void FormListOfFiles::addChildQTreeWidgetItem(QTreeWidgetItem *parent, QString parentId, QSettings *s, QStringList filesInFolders, QString path){
    QString folderPath;
    for(auto iter = filesInFolders.begin();iter!=filesInFolders.end();iter++){
        s->beginGroup(*iter);
        if(s->value("parentId").toString()==parentId){
            bool is_folder = s->value("folder").toBool();
            QString title = s->value("title").toString();
            if(is_folder){
                folderPath = path + "/" + title;
            }
            s->setValue("Path", path);
            s->setValue("filename", QString(path + "/" + title));
            QIcon icon(QString(":/fileIcons/icons/%1").arg(s->value("icon").toString()));
            s->endGroup();
            QTreeWidgetItem *itm = new QTreeWidgetItem();
            itm->setText(0, title);
            itm->setIcon(0, icon);
            parent->addChild(itm);
            if(is_folder){
                addChildQTreeWidgetItem(itm,*iter,s, filesInFolders, folderPath);
            }
        } else {
            s->endGroup();
        }
    }
}

