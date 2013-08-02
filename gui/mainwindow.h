#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//#include <QMutex>
#include <QSettings>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QFile>

#include "filesystem/formlistoffiles.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void listsObatined(FormListOfFiles *flof);
    
private slots:

//    void make_get();
//    void get_get(QNetworkReply *);

    void formfullList(FormListOfFiles *flof);
    void formList(FormListOfFiles *flof);

//    void startDownloadFile();
//    void SaveFile();
//    void downloadFinished();
//    void slotDownloadProgress(qint64,qint64);

    void on_actionForm_files_list_triggered();

    void on_actionGet_refresh_token_triggered();

    void on_actionDownload_all_files_triggered();

private:
    Ui::MainWindow *ui;


    QString ClientSecret;
    QString Code;
    QString access_token;
    QString refresh_token;

//    QNetworkAccessManager *pNetworkAccessManager_get;
    QNetworkAccessManager *pNetworkAccessManager_download_file;



};

#endif // MAINWINDOW_H
