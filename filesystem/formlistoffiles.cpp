#include "formlistoffiles.h"
#include "formfolders.h"


#include <QStringList>
#include <QDebug>


FormListOfFiles::FormListOfFiles(QTreeWidget *tw, QStatusBar *status, DataBase *db, QObject *parent) :
    QObject(parent),
    treeWidget(tw),
    statusBar(status),
    db(db)
{
    generalSettings = new QSettings("SimpleGDrive", "General");
    access_token=generalSettings->value("access_token").toString();
    networkAccessManager = new QNetworkAccessManager(this);
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getReply(QNetworkReply*)));
    isRoot=true;

    connect(this,SIGNAL(readyToFillTreeWidget()),this,SLOT(setFilesTree()));
}

void FormListOfFiles::startObtaining(){
    db->clear();
    statusBar->showMessage("Obtaining list of files...");
    getList("root");
}

void FormListOfFiles::getList(QString folderId){
    startedNetworkAccessManagers++;
    QUrl url(QString("https://www.googleapis.com/drive/v2/files?maxResults=10000&q='%1' in parents").arg(folderId));
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(access_token).toLatin1());
    networkAccessManager->get(request);
}

void FormListOfFiles::getReply(QNetworkReply *reply){
    statusBar->showMessage("Obtaining list of files...");
    finishedNetworkAccessManagers++;
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
    QString rootPath = generalSettings->value("rootDir").toString();
    QJsonDocument jd = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject globalObject = jd.object();
    QJsonArray arrayWithItems(globalObject["items"].toArray());

    for(auto iter = arrayWithItems.begin();iter!=arrayWithItems.end();  ++iter)
    {
        Data temp;
        QJsonObject file = (*iter).toObject();
        QJsonObject labels_object = file["labels"].toObject();

        if(!labels_object["trashed"].toBool()){
            temp.path=rootPath;
            temp.isRoot=true;
            temp.title=file["title"].toString();
            temp.setDate(file["modifiedDate"].toString());
            temp.setIcon(file["iconLink"].toString().mid(44));
            QJsonArray parents_array(file["parents"].toArray());
            QJsonObject parent_object = parents_array.first().toObject();
            temp.parentId=parent_object["id"].toString();
            if(file["mimeType"].toString() == "application/vnd.google-apps.folder"){
                temp.isFolder=true;
                getList(file["id"].toString());
            } else {
                if(file["mimeType"].toString().contains("google")){
                    temp.downloadUrl=file["alternateLink"].toString();
                    temp.isOnline=true;
                } else {
                    temp.downloadUrl=file["downloadUrl"].toString();
                }
                temp.originalName=file["originalFilename"].toString();
                temp.fileExtension=file["fileExtension"].toString();
                temp.md5Checksum=file["md5Checksum"].toString();
                temp.fileSize=file["fileSize"].toString().toInt();
            }
        db->dataBase.insert(file["id"].toString(),temp);
        }
    }
    generalSettings->setValue("listFormed", true);
}


void FormListOfFiles::formList(QString json){
    QJsonDocument jd = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject globalObject = jd.object();
    QJsonArray arrayWithItems(globalObject["items"].toArray());
    for(auto iter = arrayWithItems.begin();iter!=arrayWithItems.end();  ++iter)
    {
        Data temp;
        QJsonObject file = (*iter).toObject();
        QJsonObject labels_object = file["labels"].toObject();

        if(!labels_object["trashed"].toBool()){
            temp.title=file["title"].toString();
            temp.setDate(file["modifiedDate"].toString());
            temp.setIcon(file["iconLink"].toString().mid(44));
            QJsonArray parents_array(file["parents"].toArray());
            QJsonObject parent_object = parents_array.first().toObject();
            temp.parentId=parent_object["id"].toString();
            if(file["mimeType"].toString() == "application/vnd.google-apps.folder"){
                temp.isFolder=true;
                getList(file["id"].toString());
            } else {
                if(file["mimeType"].toString().contains("google")){
                    temp.downloadUrl=file["alternateLink"].toString();
                    temp.isOnline=true;
                } else {
                    temp.downloadUrl=file["downloadUrl"].toString();
                }
                temp.originalName=file["originalFilename"].toString();
                temp.fileExtension=file["fileExtension"].toString();
                temp.md5Checksum=file["md5Checksum"].toString();
                temp.fileSize=file["fileSize"].toString().toInt();
            }
            mutex.lock();
            db->dataBase.insert(file["id"].toString(),temp);
            mutex.unlock();
        }
    }
    if(startedNetworkAccessManagers==finishedNetworkAccessManagers){
        emit readyToFillTreeWidget();
    }
}

void FormListOfFiles::addRootQTreeWidgetItem(QString id, QString path){
    Data temp = db->dataBase.value(id);
    QString folderPath;
    db->dataBase[id].filename=path + "/" + db->dataBase.value(id).title;
    db->dataBase[id].isItExist();

    CustomTreeWidgetItem *itm = new CustomTreeWidgetItem(treeWidget);
    itm->setText(0, db->dataBase.value(id).title);
    itm->setIcon(0, db->dataBase.value(id).icon);
    itm->setData(1, Qt::DisplayRole, db->dataBase.value(id).modifiedDate.toLocalTime().toString("hh:mm yy-MM-dd"));
    treeWidget->addTopLevelItem(itm);
    if(temp.isFolder){
        folderPath = path + "/" + db->dataBase.value(id).title;
        addChildQTreeWidgetItem(itm, id, folderPath);
    }
}

void FormListOfFiles::addChildQTreeWidgetItem(CustomTreeWidgetItem *parent, QString parentId, QString path){
    QString folderPath;
    QMapIterator<QString, Data> i(db->dataBase);
    while (i.hasNext()) {
        i.next();
        if(i.value().parentId==parentId){
            Data temp=i.value();
            temp.path=path;
            temp.filename=path + "/" + i.value().title;
            temp.isItExist();
            db->dataBase.insert(i.key(), temp);
            CustomTreeWidgetItem *itm = new CustomTreeWidgetItem();
            itm->setText(0, i.value().title);
            itm->setIcon(0, i.value().icon);
            itm->setData(1, Qt::DisplayRole, i.value().modifiedDate.toLocalTime().toString("hh:mm yy-MM-dd"));
            parent->addChild(itm);
            if(i.value().isFolder){
                folderPath = path + "/" + i.value().title;
                addChildQTreeWidgetItem(itm, i.key(), folderPath);
            }
        }
    }
}

void FormListOfFiles::setFilesTree(bool localCall){
    treeWidget->clear();
    QString rootPath = generalSettings->value("rootDir").toString();
    QMapIterator<QString, Data> i(db->dataBase);
    while (i.hasNext()) {
        i.next();
        if(i.value().path==rootPath){
            addRootQTreeWidgetItem(i.key(), rootPath);
        }
    }
    treeWidget->setSortingEnabled(true);
    treeWidget->sortByColumn(1);

    statusBar->showMessage("List of files obtained", 600);

    if(localCall){
        db->save();
        qWarning("save");
    }
}
