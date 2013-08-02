#include "formlistoffiles.h"
#include "formfolders.h"
#include <QStringList>
#include <QDebug>

FormListOfFiles::FormListOfFiles(QObject *parent) :
    QObject(parent)
{
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
    jsonWithAllFiles = reply->readAll();
    emit fullListFormed(this);

}

void FormListOfFiles::getRootReply(QNetworkReply *reply){
    QString json = reply->readAll();
    //    qWarning() << json;
    formRootList(json);
}

void FormListOfFiles::formRootList(QString json){

    QStringList folderListInRoot, rootList;
    QSettings s("SimpleDrive", "Files");
    s.setIniCodec("UTF-8");
    s.clear();
    QJsonDocument jd = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject globalObject = jd.object();
    QJsonArray arrayWithItems(globalObject["items"].toArray());
    for(auto iter = arrayWithItems.begin();iter!=arrayWithItems.end();  ++iter)
    {
        QJsonObject file = (*iter).toObject();

        s.beginGroup(file["id"].toString());
        s.setValue("title", file["title"].toString());
        s.setValue("modifiedDate", file["modifiedDate"].toString());
        s.setValue("iconLink", file["iconLink"].toString());
        s.setValue("parentId", "root");
        if(file["mimeType"].toString() == "application/vnd.google-apps.folder"){
            folderListInRoot.push_back(file["id"].toString());
        } else {
            rootList.push_back(file["id"].toString());
            s.setValue("downloadUrl", file["downloadUrl"].toString());//.replace("\"", ""));
            s.setValue("originalFilename", file["originalFilename"].toString());
            s.setValue("fileExtension", file["fileExtension"].toString());
            s.setValue("md5Checksum", file["md5Checksum"].toString());
            s.setValue("fileSize", file["fileSize"].toString());
        }
        //        s.setValue("", asdw[""].toString());
        s.endGroup();
    }

    if(!folderListInRoot.empty()) s.setValue("foldersInRoot", folderListInRoot);
    if(!rootList.empty()) s.setValue("rootFiles", rootList);
    //    FormFolders *ff = new FormFolders();
    //    if(!folderListInRoot.isEmpty()) ff->getFolderList(folderListInRoot.first());
    emit rootListFormed(this);
}

void FormListOfFiles::formList(){
    QSettings s("SimpleDrive", "Files");
    s.setIniCodec("UTF-8");
    QStringList checklist = s.value("foldersInRoot").toStringList();
    QStringList filesInFolders;
    QJsonDocument jd = QJsonDocument::fromJson(jsonWithAllFiles.toUtf8());
    QJsonObject globalObject = jd.object();
    QJsonArray arrayWithItems(globalObject["items"].toArray());
    for(auto iter = arrayWithItems.begin();iter!=arrayWithItems.end();  ++iter)
    {
        QJsonObject file = (*iter).toObject();

        QJsonObject labels_object = file["labels"].toObject();
                if(!labels_object["trashed"].toBool()){
        s.beginGroup(file["id"].toString());
        filesInFolders.push_back(file["id"].toString());
        s.setValue("title", file["title"].toString());
        s.setValue("modifiedDate", file["modifiedDate"].toString());
        s.setValue("iconLink", file["iconLink"].toString());
        QJsonArray parents_array(file["parents"].toArray());
        QJsonObject parent_object = parents_array.first().toObject();
        s.setValue("parentId", parent_object["id"].toString());
        if(file["mimeType"].toString() == "application/vnd.google-apps.folder"){
            s.setValue("folder",true);
        } else {
            //            rootList.push_back(file["id"].toString());
            s.setValue("downloadUrl", file["downloadUrl"].toString());
            s.setValue("originalFilename", file["originalFilename"].toString());
            s.setValue("fileExtension", file["fileExtension"].toString());
            s.setValue("md5Checksum", file["md5Checksum"].toString());
            s.setValue("fileSize", file["fileSize"].toString());
        }
        //        s.setValue("", asdw[""].toString());
        s.endGroup();
    }
    }
    if(!filesInFolders.isEmpty()) s.setValue("filesInFolders", filesInFolders);
}
