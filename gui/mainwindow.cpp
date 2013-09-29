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


// TODO: Make check for drive folder.


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    MultipartUpload *mu = new MultipartUpload();
//    mu->startUpload("/home/s/test.deb");

    setSettings();
    setFilesTree();

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
    QSettings s("SimpleDrive", "General");
    ui->LogedAs->setText("You logged in as:\t"+s.value("email").toString());
    ui->rootDirectory->setText(s.value("rootDir").toString());
    QTimer::singleShot(120000, this, SLOT(setSettings()));
}

void MainWindow::setFilesTree(){
    QSettings generalSettings("SimpleDrive", "General");
    if(generalSettings.value("listFormed").toBool()){
        FormListOfFiles flof(ui->treeWidget);
        QSettings s("SimpleDrive", "Files");
        s.setIniCodec("UTF-8");
        QStringList rootFolders = s.value("Folders").toStringList();
        QStringList rootFiles = s.value("Files").toStringList();
        for(auto iter = rootFolders.begin();iter!=rootFolders.end();iter++){
            flof.addRootQTreeWidgetItem(*iter,&s, generalSettings.value("rootDir").toString());
        }
        for(auto iter = rootFiles.begin();iter!=rootFiles.end();iter++){
            flof.addRootQTreeWidgetItem(*iter,&s, generalSettings.value("rootDir").toString());
        }
        ui->treeWidget->setSortingEnabled(true);
        ui->treeWidget->sortByColumn(0);
    }
}

void MainWindow::refreshToken()
{
    qWarning("refresh");
    Authorization *auth = new Authorization();
    auth->refreshToken();
}


void MainWindow::on_actionForm_files_list_triggered()
{

    FormListOfFiles *flof = new FormListOfFiles(ui->treeWidget);
    flof->startObtaining();
}



void MainWindow::on_actionDownload_all_files_triggered()
{
    QSettings s("SimpleDrive", "General");
    FormFolders *d = new FormFolders(ui->statusBar, ui->progressBar);
    d->makeRootFolder(s.value("rootDir").toString()); // TODO: Refactor that fucntions parameters

}

void MainWindow::on_chooseDirButton_clicked()
{
    QString rootdir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->rootDirectory->setText(rootdir);
    QSettings s("SimpleDrive", "General");
    s.setValue("rootDir", rootdir);
}

void MainWindow::logout(){
//     FIXME: It's just restarting application. Not logging out. I think i need another way to do it.
    QSettings s("SimpleDrive", "General");
    s.remove("access_token");
    s.remove("refresh_token");

    QProcess::startDetached(QApplication::applicationFilePath());
    exit(12);

}
