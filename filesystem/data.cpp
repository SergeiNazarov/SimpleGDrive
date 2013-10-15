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
    gDate=gDate.replace(20, 3, "000");
    modifiedDate = QDateTime::fromString(gDate, "yyyy-MM-ddThh:mm:ss.zzzZ");
    modifiedDate.setTimeSpec(Qt::UTC);
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

bool Data::download(){
    if(exist){
//        qWarning("exist");
        if(isMd5ChecksumCoincide()){
//            qWarning("md5 true");
            return false;
        } else {
//            qWarning("md5 false");
            if(isLocalModifiedDateEarlier()){
//                qWarning("date erlier");
                return true;
            } else {
//                qWarning("date later");
                return false;
            }
        }
    } else {
//        qWarning("not exist");
        return true;
    }

}

bool Data::isMd5ChecksumCoincide(){
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();
        QByteArray hashData = QCryptographicHash::hash(fileData, QCryptographicHash::Md5);

        if(hashData.toHex()==md5Checksum)
            return true;
        else
            return false;
    } else {
        return isMd5ChecksumCoincide();
    }
}

bool Data::isLocalModifiedDateEarlier(){
    QFile file(filename);
    QFileInfo info(file);
    if(info.lastModified().toUTC()<modifiedDate)
        return true;
    else
        return false;
}

void Data::setModifiedDate(){
    QString temp=filename;
    for(int i=1; i<temp.length();i+=2){
        temp=temp.insert(i, '\\');
    }
    QStringList arguments;
    arguments << "-c" << QString("touch -mt %1 %2").arg(modifiedDate.toLocalTime().toString("yyyyMMddhhmm.ss")).arg(temp);
    QProcess exec;
    exec.start("/bin/sh", arguments);
    while(exec.waitForFinished()){}
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
