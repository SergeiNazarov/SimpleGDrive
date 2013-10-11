#ifndef DATA_H
#define DATA_H

#include <QIcon>
#include <QFile>
#include <QDateTime>
#include <QMetaType>

class Data
{
public:
    Data();

    void isItExist();
    void setDate(QString gDate);
    QString getDate();
    void setIcon(QString icon);
    void display();

    QString title;
    bool isFolder=false;
    QString path;
    QString downloadUrl;
    QString fileExtension;
    QString md5Checksum;
    qint64 fileSize;
    QIcon icon;
    QString filename;
    QString originalName;
    QString parentId;
    bool exist=false;
    QDateTime modifiedDate;
    bool isOnline=false;
    bool isRoot=false;

private:



};

Q_DECLARE_METATYPE(Data)
QDataStream& operator << (QDataStream& out, const Data& obj);
QDataStream& operator >> (QDataStream& in, Data& obj);

#endif // DATA_H

/*
 *
 *
 *
 *[0B2vO_SsrmmyhRDg3YjZQSFVvbjg]
Path=/home/s/Drive/TESTER/DOC
download=true
filename=/home/s/Drive/TESTER/DOC/ASM&DOS1
folder=true
icon=icon_11_collection_list.png
modifiedDate=2013-09-22T14:17:55.500Z
parentId=0B2vO_SsrmmyhcVFHSlZZQm8wVjQ
title=ASM&DOS1



 *[0B2vO_SsrmmyhOFhNWHBVMEVDTFk]
    Path=/home/s/Drive/TESTER/DOC/386TECH
    download=true
    downloadUrl="https://doc-0s-10-docs.googleusercontent.com/docs/securesc/vh43f00h6vomsbbn33jmvn5j8ivason2/
                    8dkqq3ke8ig2s63ar1eg1s7u6ms61op9/1380729600000/13577424772389623778/13577424772389623778/0B2vO_SsrmmyhOFhNWHBVMEVDTFk?h=16653014193614665626&e=download&gd=true"
    fileExtension=txt
    fileSize=105600
    filename=/home/s/Drive/TESTER/DOC/386TECH/TECH2.TXT
    icon=icon_10_text_list.png
    md5Checksum=b496c36c0f0a1ef5a69fbe370053a10c
    modifiedDate=2013-09-22T14:18:44.897Z
    originalFilename=TECH2.TXT
    parentId=0B2vO_SsrmmyhU3RzUUd6LWx6YTg
    title=TECH2.TXT



*/
