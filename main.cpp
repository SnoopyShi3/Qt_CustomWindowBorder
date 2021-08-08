#include "mainwindow.h"

#include <QApplication>
#include "BaseBorderWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    BaseBorderWidget customBorder(new MainWindow);
    customBorder.show();

    return a.exec();
}
