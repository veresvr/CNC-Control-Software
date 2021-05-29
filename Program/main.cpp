#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName( "VR" );
    QApplication::setOrganizationDomain( "___________" );
    QApplication::setApplicationName( "CNC Control" );
 //   QApplication::setWindowIcon(QPixmap(QCoreApplication::applicationDirPath() + "/img/main.png"));
QApplication::setWindowIcon(QIcon(":logo.svg"));


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
