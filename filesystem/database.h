#ifndef DATABASE_H
#define DATABASE_H

#include <QMap>
#include <QMetaType>
#include <QSettings>

#include "data.h"

class DataBase
{
public:
    DataBase();
    void countDownloadedFiles();
    void countAllFiles();
    void countDriveSize();
    void verifyExisting();
    void display();
    void save();
    void load();

    int downloadedFiles;
    int filesQuantity;
    qint64 driveSize;
    QMap<QString, Data> dataBase;
};

Q_DECLARE_METATYPE(DataBase)

QDataStream& operator << (QDataStream& out, const DataBase& obj);
QDataStream& operator >> (QDataStream& in, DataBase& obj);

#endif // DATABASE_H
