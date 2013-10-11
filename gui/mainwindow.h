#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QFile>

#include "filesystem/formlistoffiles.h"
#include "filesystem/database.h"

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

    
private slots:
    void refreshToken();
    void setSettings();
    void setFilesTree();

    void on_actionForm_files_list_triggered();

    void on_actionDownload_all_files_triggered();

    void on_chooseDirButton_clicked();
    void logout();

private:
    QSettings *generalSettings;

    DataBase *db;

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
