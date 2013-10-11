#include "initiation.h"
#include <QApplication>
#include "gui/mainwindow.h"
#include <QNetworkAccessManager>
#include "filesystem/database.h"
#include "filesystem/data.h"


int main(int argc, char *argv[])
{
    qRegisterMetaTypeStreamOperators<DataBase>("DataBase");
    qRegisterMetaTypeStreamOperators<Data>("Data");
    QApplication a(argc, argv);

    MainWindow *w = new MainWindow();
    Initiation *init = new Initiation(w);

    return a.exec();
}
