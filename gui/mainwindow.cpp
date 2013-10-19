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

     connect(ui->rootDirectory, SIGNAL(textChanged(QString)), this, SLOT(setSettings()));

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

    if(ui->rootDirectory->text()!="") {
        ui->actionForm_files_list->setEnabled(true);
        ui->actionForm_files_list->setToolTip("Form files list");
    }
    if(ui->rootDirectory->text()!="" && generalSettings->value("listFormed").toBool()) {
        ui->actionDownload_all_files->setEnabled(true);
        ui->actionDownload_all_files->setToolTip("Download all files");
    }
}

void MainWindow::setUI(){
    if(generalSettings->value("listFormed").toBool()){
        FormListOfFiles *flof = new FormListOfFiles(ui->treeWidget, ui->statusBar, ui->progressBar, db);
        flof->setFilesTree(false);
    }
    ui->label->setOpenExternalLinks(true);
}

void MainWindow::refreshToken()
{
    qWarning("refresh token");
    Authorization *auth = new Authorization();
    auth->refreshToken();
}

void MainWindow::on_actionForm_files_list_triggered()
{
    FormListOfFiles *flof = new FormListOfFiles(ui->treeWidget, ui->statusBar,ui->progressBar, db);
    flof->startObtaining();
    ui->actionDownload_all_files->setEnabled(true);
    ui->actionDownload_all_files->setToolTip("Download all files");
}

void MainWindow::on_actionDownload_all_files_triggered()
{
    FormFolders *d = new FormFolders(db, ui->statusBar, ui->progressBar);
    d->makeRootFolder(generalSettings->value("rootDir").toString()); // TODO: Refactor that fucntions parameters
}

void MainWindow::on_chooseDirButton_clicked()
{
    QString rootdir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    generalSettings->setValue("rootDir", rootdir);
    ui->rootDirectory->setText(rootdir);
}

void MainWindow::logout(){
//     FIXME: It's just restarting application. Not logging out. I think i need another way to do it.
    generalSettings->remove("access_token");
    generalSettings->remove("refresh_token");

    QProcess::startDetached(QApplication::applicationFilePath());
    exit(12);
}
