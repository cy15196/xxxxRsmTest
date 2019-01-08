#include "rsmconfigmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Rsm");
    a.setApplicationName("RsmConfigure");
    RSMConfigMainWindow w;
    w.show();

    return a.exec();
}
