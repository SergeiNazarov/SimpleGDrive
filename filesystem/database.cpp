#include "database.h"

#include <QDebug>

DataBase::DataBase()
{

}

void DataBase::countDownloadedFiles(){
    downloadedFiles=0;
    foreach (Data data, dataBase) {
        if(data.exist && !data.isFolder && !data.isOnline){
            downloadedFiles++;
        }
    }
}

void DataBase::countAllFiles(){
    filesQuantity=0;
    foreach (Data data, dataBase) {
        if(!data.isFolder && !data.isOnline){
            filesQuantity++;
        }
    }
}

void DataBase::countDriveSize(){
    driveSize=0;
    foreach (Data data, dataBase) {
        if(!data.isFolder && !data.isOnline){
            driveSize+=data.fileSize;
        }
    }
}

void DataBase::verifyExisting(){
    foreach (Data data, dataBase) {
        data.isItExist();
    }
}

void DataBase::display(){
    foreach (Data data, dataBase) {
        data.display();
    }
}

void DataBase::save(){
    QSettings settings("SimpleGDrive", "Files");
    settings.clear();
    QVariant variant;
    variant.setValue(QVariant::fromValue<DataBase>(*this));
    settings.setValue("Data", variant);
}

void DataBase::load(){
    QSettings settings("SimpleGDrive", "Files");
    *this = settings.value("Data").value<DataBase>();
}

void DataBase::clear(){
    downloadedFiles=0;
    filesQuantity=0;
    driveSize=0;
    dataBase.clear();
}

QDataStream& operator << (QDataStream& out, const DataBase& obj){
    out << obj.dataBase << obj.downloadedFiles << obj.filesQuantity << obj.driveSize;
    return out;
}

    QDataStream& operator >> (QDataStream& in, DataBase& obj){
    in >> obj.dataBase >> obj.downloadedFiles >> obj.filesQuantity >> obj.driveSize;
    return in;
}
