#include <QCoreApplication>
#include <QTimer>

#include "cinkciarzapplication.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CinkciarzApplication application;

    QObject::connect(&application,
                     &CinkciarzApplication::finished,
                     &a,
                     &QCoreApplication::quit);

    QTimer::singleShot(10, &application, SLOT(run()));
    return a.exec();
}
