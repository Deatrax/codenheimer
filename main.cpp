#include "mainwindow.h"

#include <QApplication>
#include <QSettings>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //exampleUsage();
    a.setWindowIcon(QIcon(":/images/codenheimer_icon3.png"));
    MainWindow::firstTimeInit();
    MainWindow w;
    w.show();
    return a.exec();
}
