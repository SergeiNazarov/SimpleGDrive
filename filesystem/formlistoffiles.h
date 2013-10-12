#ifndef FORMLISTOFFILES_H
#define FORMLISTOFFILES_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTreeWidget>
#include <QMutex>
#include <QStatusBar>

#include "data.h"
#include "database.h"

class CustomTreeWidgetItem : public QTreeWidgetItem
{
public:
    CustomTreeWidgetItem(QTreeWidget *tree) : QTreeWidgetItem(tree)  {}
    CustomTreeWidgetItem(QTreeWidget * parent, const QStringList & strings)
                   : QTreeWidgetItem (parent,strings)  {}
    CustomTreeWidgetItem(){}
    bool operator< (const QTreeWidgetItem &other) const
    {
        int column = treeWidget()->sortColumn();
        if(column==1){
            QDateTime first = QDateTime::fromString(text(column), "hh:mm yy-MM-dd");
            QDateTime otherdate = QDateTime::fromString(other.text(column), "hh:mm yy-MM-dd");
            return first < otherdate;
        } else {
            const QVariant v1 = data(column, Qt::DisplayRole);
            const QVariant v2 = other.data(column, Qt::DisplayRole);
            return v1.toString() < v2.toString();
        }
    }

};

class FormListOfFiles : public QObject
{
    Q_OBJECT
public:
    explicit FormListOfFiles(QTreeWidget *tw = 0, QStatusBar *status = 0, DataBase *db = 0, QObject *parent = 0);
    void getFullList();
    void addRootQTreeWidgetItem(QString id, QString path);

signals:
    void fullListFormed(FormListOfFiles*);
    void readyToFillTreeWidget();
    
public slots:
    void startObtaining();
    void setFilesTree(bool localCall = true);

private slots:
    void getReply(QNetworkReply *);

private:
    void getList(QString folderId);
    void addChildQTreeWidgetItem(CustomTreeWidgetItem *parent, QString parentId, QString path);

    void formRootList(QString json);
    void formList(QString json);

    QString access_token;
    QString jsonWithAllFiles;
    QStringList FoldersList, FilesList, filesInFolders;
    QString bigJson;

    int startedNetworkAccessManagers=0;
    int finishedNetworkAccessManagers=0;

    QMutex mutex;

    QNetworkAccessManager *networkAccessManager;

    QSettings *generalSettings;

    QTreeWidget *treeWidget;

    QStatusBar *statusBar;

    DataBase *db;

    bool isRoot;
    
};


#endif // FORMLISTOFFILES_H
