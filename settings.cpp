#include "settings.h"
#include <QDebug>

Settings::Settings()
{
    s=new QSettings("SimpleDrive", "General");
}

void Settings::set(QString name, QString value){
    s->setValue(name, value);
}
