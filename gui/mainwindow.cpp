#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network/authorization.h"
#include "filesystem/formlistoffiles.h"
#include "network/downloadfile.h"
#include <QDir>
#include "filesystem/formfolders.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ClientSecret = "sX7mhNqmBjmMtQqfznn7zRwd";

//    ui->treeWidget->



    pNetworkAccessManager_download_file = new QNetworkAccessManager(this);

    //    connect(pNetworkAccessManager_download_file, SIGNAL(finished(QNetworkReply*)), this, SLOT(SaveFile(QNetworkReply*)));
    connect(this, SIGNAL(listsObatined(FormListOfFiles*)), this, SLOT(formList(FormListOfFiles*)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::formList(FormListOfFiles *flof){
    flof->formList();
    qWarning("I'min main form list");
}





void MainWindow::on_actionForm_files_list_triggered()
{
    FormListOfFiles *flof = new FormListOfFiles();
    flof->getRootList();
    connect(flof, SIGNAL(rootListFormed(FormListOfFiles*)), this,SLOT(formfullList(FormListOfFiles*)));
}

void MainWindow::formfullList(FormListOfFiles *flof){
    flof->getFullList();
    connect(flof, SIGNAL(fullListFormed(FormListOfFiles*)), this,SLOT(formList(FormListOfFiles*)));
}

void MainWindow::on_actionGet_refresh_token_triggered()
{
    Authorization *auth = new Authorization();
    auth->refreshToken();
}

void MainWindow::on_actionDownload_all_files_triggered()
{

    FormFolders *d = new FormFolders();
    d->makeRootFolder();
//    QDir dir("/home/s/");
//    qWarning()<<dir.absolutePath();
    }
