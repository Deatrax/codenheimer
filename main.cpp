#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/codenheimer_icon3.png"));
    MainWindow::firstTimeInit();
    MainWindow w;
    w.show();
    return a.exec();
}
