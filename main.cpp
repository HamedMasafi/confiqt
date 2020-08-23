#include "qtconfigwizard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("ConfiQt");

    QtConfigWizard w;
    w.show();

    return a.exec();
}
