#include "formlistoffiles.h"
#include "formfolders.h"
#include <QStringList>
#include <QDebug>


FormListOfFiles::FormListOfFiles(QTreeWidget *tw, QObject *parent) :
    QObject(parent),
    treeWidget(tw)
{
    treeWidget->clear();
    QSettings settings("SimpleDrive", "General");
    access_token=settings.value("access_token").toString();
    pNetworkAccessManager = new QNetworkAccessManager(this);
    connect(pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getReply(QNetworkReply*)));
    pNetworkAccessManager_root = new QNetworkAccessManager(this);
    connect(pNetworkAccessManager_root, SIGNAL(finished(QNetworkReply*)), this, SLOT(getRootReply(QNetworkReply*)));
}


void FormListOfFiles::getFullList(){
    QString s = QString("https://www.googleapis.com/drive/v2/files");
    QUrl url_get(s);
    QNetworkRequest request_get;
    request_get.setUrl(url_get);
    request_get.setRawHeader("Authorization", QString("Bearer %1").arg(access_token).toLatin1());
    pNetworkAccessManager->get(request_get);
}

void FormListOfFiles::getRootList(){
    QSettings settings("SimpleDrive", "General");
    access_token=settings.value("access_token").toString();
    QString s = QString("https://www.googleapis.com/drive/v2/files?q='root' in parents");
    QUrl url_get(s);
    QNetworkRequest request_get;
    request_get.setUrl(url_get);
    request_get.setRawHeader("Authorization", QString("Bearer %1").arg(access_token).toLatin1());
    pNetworkAccessManager_root->get(request_get);
}

void FormListOfFiles::getReply(QNetworkReply *reply){
    QString json = reply->readAll();
    formList(json);

}

void FormListOfFiles::getRootReply(QNetworkReply *reply){
    QString json = reply->readAll();
    formRootList(json);
}

// TODO: Refactor formRootList fucntions and function FormFolders::makeRootFolder.
void FormListOfFiles::formRootList(QString json){
    QStringList rootFoldersList, rootList;
    QSettings generalSettings("SimpleDrive", "General");
    QString rootPath = generalSettings.value("rootDir").toString();
    QSettings s("SimpleDrive", "Files");
    s.setIniCodec("UTF-8");
    QJsonDocument jd = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject globalObject = jd.object();
    QJsonArray arrayWithItems(globalObject["items"].toArray());
    for(auto iter = arrayWithItems.begin();iter!=arrayWithItems.end();  ++iter)
    {
        QJsonObject file = (*iter).toObject();
        s.beginGroup(file["id"].toString());
        s.setValue("Path", rootPath);
        s.setValue("title", file["title"].toString());
        s.setValue("modifiedDate", file["modifiedDate"].toString());
        s.setValue("icon", file["iconLink"].toString().mid(44));
        s.setValue("parentId", "root");
        if(file["mimeType"].toString() == "application/vnd.google-apps.folder"){
            rootFoldersList.push_back(file["id"].toString());
        } else {
            rootList.push_back(file["id"].toString());
            if(file["mimeType"].toString().contains("google")){
                //                TODO: Save links as a file for online files.
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
        addRootQTreeWidgetItem(file["id"].toString(), &s, rootPath);
    }

    if(!rootFoldersList.empty()) s.setValue("rootFolders", rootFoldersList);
    if(!rootList.empty()) s.setValue("rootFiles", rootList);
    //    FormFolders *ff = new FormFolders();
    //    if(!folderListInRoot.isEmpty()) ff->getFolderList(folderListInRoot.first());
    cleanUpFilesList();
    generalSettings.setValue("listFormed", true);
}

void FormListOfFiles::formList(QString json){
    QSettings s("SimpleDrive", "Files");
    s.setIniCodec("UTF-8");
    s.clear();
    QJsonDocument jd = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject globalObject = jd.object();
    QJsonArray arrayWithItems(globalObject["items"].toArray());
    for(auto iter = arrayWithItems.begin();iter!=arrayWithItems.end();  ++iter)
    {
        QJsonObject file = (*iter).toObject();
        QJsonObject labels_object = file["labels"].toObject();

        if(!labels_object["trashed"].toBool()){
            s.beginGroup(file["id"].toString());
            filesInFolders.push_back(file["id"].toString());
            s.setValue("download", true);
            s.setValue("title", file["title"].toString());
            s.setValue("modifiedDate", file["modifiedDate"].toString());
            s.setValue("icon", file["iconLink"].toString().mid(44));
            QJsonArray parents_array(file["parents"].toArray());
            QJsonObject parent_object = parents_array.first().toObject();
            s.setValue("parentId", parent_object["id"].toString());
            if(file["mimeType"].toString() == "application/vnd.google-apps.folder"){
                s.setValue("folder",true);
            } else {
                if(file["mimeType"].toString().contains("google")){
                    //  TODO: Save links as a file for online files here too.
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
    if(!filesInFolders.isEmpty()) s.setValue("filesInFolders", filesInFolders);
    emit fullListFormed(this);
}

void FormListOfFiles::addRootQTreeWidgetItem(QString id, QSettings *s, QString path){
    QString folderPath;
    s->beginGroup(id);
    bool is_folder = s->value("folder").toBool();
    QString title = s->value("title").toString();
    if(is_folder){
        folderPath = path + "/" + title;
    }
    QIcon icon(QString(":/fileIcons/icons/%1").arg(s->value("icon").toString()));
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

void FormListOfFiles::cleanUpFilesList(){
    QSettings s("SimpleDrive", "Files");
    s.setIniCodec("UTF-8");
    QStringList FilesList = s.value("filesInFolders").toStringList();
    QStringList cleanedFilesList;
    for(auto iter = FilesList.begin();iter!=FilesList.end();iter++){
        s.beginGroup(*iter);
        if(s.contains("Path")){
            s.endGroup();
            cleanedFilesList.push_back(*iter);
        } else {
            s.endGroup();
            s.remove(*iter);
        }
    }
    s.setValue("filesInFolders", cleanedFilesList);
}
