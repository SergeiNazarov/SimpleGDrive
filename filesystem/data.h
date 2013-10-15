#ifndef DATA_H
#define DATA_H

#include <QIcon>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QMetaType>
#include <QProcess>
#include <QCryptographicHash>


class Data
{
public:
    Data();

    void isItExist();
    void setDate(QString gDate);
    QString getDate();
    void setIcon(QString icon);
    void display();
    bool download();
    bool isMd5ChecksumCoincide();
    bool isLocalModifiedDateEarlier();
    void setModifiedDate();

    QString title;
    bool isFolder=false;
    QString path;
    QString downloadUrl;
    QString fileExtension;
    QString md5Checksum;
    qint64 fileSize=0;
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
