#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //exampleUsage();
    a.setWindowIcon(QIcon(":/images/codenheimer_icon3.png"));
    int ret=MainWindow::firstTimeInit();
    if(ret==-1){
        return -1;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
