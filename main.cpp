#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    std::cout<<QApplication::applicationDirPath().toLocal8Bit().constData();
    return a.exec();
}
