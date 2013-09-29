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


class FormListOfFiles : public QObject
{
    Q_OBJECT
public:
    explicit FormListOfFiles(QTreeWidget *tw = 0, QObject *parent = 0);
    void getFullList();
    void addRootQTreeWidgetItem(QString id, QSettings *s, QString path);

signals:
    void fullListFormed(FormListOfFiles*);
    
public slots:
    void startObtaining();

private slots:

    void getReply(QNetworkReply *);

private:
    void getList(QString folderId);
    void addChildQTreeWidgetItem(QTreeWidgetItem *parent, QString parentId, QSettings *s, QStringList filesInFolders, QString path);

    void formRootList(QString json);
    void formList(QString json);

    int ijk;
    QString access_token;
    QString jsonWithAllFiles;
    QStringList filesInFolders;
    QString bigJson;

    QMutex mutex;

    QNetworkAccessManager *NetworkAccessManager;
    QNetworkAccessManager *pNetworkAccessManager;

    QSettings *Settings;

    QTreeWidget *treeWidget;

    bool isRoot;
    
};

#endif // FORMLISTOFFILES_H
