#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network/authorization.h"
#include "filesystem/formlistoffiles.h"
#include "network/downloadfile.h"
#include "filesystem/formfolders.h"

#include "network/multipartupload.h"

#include <QDir>
#include <QTimer>
#include <QFileDialog>
#include <QProcess>
#include <QThread>
#include <QCryptographicHash>
#include <QDateTime>
#include <QFileInfo>


#include <filesystem/data.h>


// TODO: Make check for drive folder.


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new DataBase();
    db->load();

//    db->display();

    generalSettings = new QSettings("SimpleGDrive", "General");

//    MultipartUpload *mu = new MultipartUpload();
//    mu->startUpload("/home/s/test.deb");

    setSettings();
    setUI();

    refreshToken();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshToken()));
    timer->start(3500000);

    connect(ui->logoutButton, SIGNAL(clicked()), this, SLOT(logout()));

     ui->statusBar->showMessage(tr("Ready"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSettings(){
    qWarning("setSettings");
    ui->LogedAs->setText("You logged in as:\t"+generalSettings->value("email").toString());
    ui->rootDirectory->setText(generalSettings->value("rootDir").toString());
    if(!generalSettings->contains("refresh_token")) QTimer::singleShot(120000, this, SLOT(setSettings()));
}

void MainWindow::setUI(){
    if(db->filesQuantity!=0)
        ui->progressBar->setValue((db->downloadedFiles)/(db->filesQuantity)*100);
    else
        ui->progressBar->setValue(0);

    if(generalSettings->value("listFormed").toBool()){
        FormListOfFiles *flof = new FormListOfFiles(ui->treeWidget, ui->statusBar, db);
        flof->setFilesTree(false);
    }
}

void MainWindow::refreshToken()
{
    qWarning("refresh token");
    Authorization *auth = new Authorization();
    auth->refreshToken();
}

void MainWindow::on_actionForm_files_list_triggered()
{
    FormListOfFiles *flof = new FormListOfFiles(ui->treeWidget, ui->statusBar, db);
    flof->startObtaining();
}

void MainWindow::on_actionDownload_all_files_triggered()
{
    FormFolders *d = new FormFolders(db, ui->statusBar, ui->progressBar);
    d->makeRootFolder(generalSettings->value("rootDir").toString()); // TODO: Refactor that fucntions parameters
}

void MainWindow::on_chooseDirButton_clicked()
{
    QString rootdir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->rootDirectory->setText(rootdir);
    generalSettings->setValue("rootDir", rootdir);
}

void MainWindow::logout(){
//     FIXME: It's just restarting application. Not logging out. I think i need another way to do it.
    generalSettings->remove("access_token");
    generalSettings->remove("refresh_token");

    QProcess::startDetached(QApplication::applicationFilePath());
    exit(12);
}
