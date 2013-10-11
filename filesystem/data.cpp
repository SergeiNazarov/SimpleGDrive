#include "data.h"
#include <QDebug>
#include <QDate>
#include <QTime>

Data::Data()
{
}

void Data::isItExist(){
    QFile temp(filename);
    if(temp.exists()){
        exist=true;
    } else {
        exist=false;
    }
}


void Data::setDate(QString gDate){
    QDate date;
    int year = gDate.mid(0,4).toInt();
    int month = gDate.mid(5, 2).toInt();
    int day = gDate.mid(8, 2).toInt();
    date.setDate(year,month,day);
    QTime time;
    int hours = gDate.mid(11,2).toInt();
    int minutes = gDate.mid(14,2).toInt();
    int seconds = gDate.mid(17,2).toInt();
    int miliseconds = gDate.mid(20,3).toInt();
    time.setHMS(hours, minutes, seconds, miliseconds);
    modifiedDate.setDate(date);
    modifiedDate.setTime(time);
//    qWarning()<<modifiedDate.toString();
}


void Data::setIcon(QString icon){
    QFile file(QString(":/fileIcons/icons/%1").arg(icon));
    if(file.exists()){
        this->icon = QIcon(QString(":/fileIcons/icons/%1").arg(icon));
    } else {
        this->icon = QIcon(QString(":/fileIcons/icons/icon_10_generic_list.png"));
    }
}

QString Data::getDate(){
    return modifiedDate.toString("yyyy-MM-ddThh:mm:ss.zzzZ");
}

void Data::display(){
    qWarning()<<"\n\ntitle"<<title;
    qWarning()<<"isFolder"<<isFolder;
    qWarning()<<"path"<<path;
    qWarning()<<"downloadUrl"<<downloadUrl;
    qWarning()<<"fileextension"<<fileExtension;
    qWarning()<<"md5"<<md5Checksum;
    qWarning()<<"filesize"<<fileSize;
    qWarning()<<"icon"<<icon.name();
    qWarning()<<"filename"<<filename;
    qWarning()<<"origialName"<<originalName;
    qWarning()<<"parentId"<<parentId;
    qWarning()<<"exist"<<exist;
    qWarning()<<"modified Date"<<modifiedDate;
    qWarning()<<"online"<<isOnline;
    qWarning()<<"root"<<isRoot;

}

QDataStream& operator << (QDataStream& out, const Data& obj)
{
    out << obj.title << obj.isFolder << obj.path << obj.downloadUrl << obj.fileExtension << obj.md5Checksum << obj.fileSize << obj.icon << obj.filename
            << obj.originalName << obj.parentId << obj.exist << obj.modifiedDate << obj.isOnline << obj.isRoot;
    return out;
}

QDataStream& operator >> (QDataStream& in, Data& obj)
{
    in >> obj.title >> obj.isFolder >> obj.path >> obj.downloadUrl >> obj.fileExtension >> obj.md5Checksum >> obj.fileSize >> obj.icon >> obj.filename
            >> obj.originalName >> obj.parentId >> obj.exist >> obj.modifiedDate >> obj.isOnline >> obj.isRoot;
    return in;
}


//bool Data::operator ==(const Data& d) const{
//    if(this->downloadUrl==d.downloadUrl && this->exist==d.exist && this->fileExtension==d.fileExtension &&
//            this->filename==d.filename && this->fileSize==d.fileSize && this->icon==d.icon && this->isFolder==d.isFolder &&
//            this->md5Checksum==d.md5Checksum && this->modifiedDate==d.modifiedDate && this->online==d.online &&
//            this->originalName==d.originalName && this->parentId==d.parentId && this->path==d.path && this->title==d.title)
//        return true;
//    else
//        return false;

//}
