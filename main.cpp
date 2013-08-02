#include "initiation.h"
#include <QApplication>
#include "gui/mainwindow.h"
#include <QNetworkAccessManager>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow *w = new MainWindow();
    Initiation *init = new Initiation(w);

    return a.exec();
}
