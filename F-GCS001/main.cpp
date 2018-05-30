#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //a.setQuitOnLastWindowClosed(false);

    //QCoreApplication::processEvents();

    w.show();

    return a.exec();
}
