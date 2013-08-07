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


class FormListOfFiles : public QObject
{
    Q_OBJECT
public:
    explicit FormListOfFiles(QTreeWidget *tw, QObject *parent = 0);
    void getRootList();
    void getFullList();
    void addRootQTreeWidgetItem(QString id, QSettings *s, QString path);

signals:
    void fullListFormed(FormListOfFiles*);
    
public slots:

private slots:

    void getRootReply(QNetworkReply *);
    void getReply(QNetworkReply *);

private:
    void addChildQTreeWidgetItem(QTreeWidgetItem *parent, QString parentId, QSettings *s, QStringList filesInFolders, QString path);
    void cleanUpFilesList();

    void formList(QString json);
    void formRootList(QString json);

    QString access_token;
    QString jsonWithAllFiles;
    QStringList filesInFolders;

    QNetworkAccessManager *pNetworkAccessManager_root;
    QNetworkAccessManager *pNetworkAccessManager;

    QTreeWidget *treeWidget;
    
};

#endif // FORMLISTOFFILES_H
