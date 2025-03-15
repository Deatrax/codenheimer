#include <QApplication>
#include <QProcess>
#include "mainwindow.h"
#include "welcomescreen.h"

void restartApplication() {
    QString program = QCoreApplication::applicationFilePath();
    QProcess::startDetached(program, QStringList());  // Relaunch app
    QApplication::quit();  // Close current instance
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/codenheimer_icon3.png"));

    int ret = MainWindow::firstTimeInit();
    if (ret == -1) {
        return -1;  // Exit if first-time setup fails
    }

    if (ret == 1) {
        // Launch onboarding flow
        welcomeScreen wel;
        wel.show();

        int exitCode = a.exec();  // Run onboarding
        if (exitCode == 123) {    // Use a special code for restart
            restartApplication();
        }
        return exitCode;
    }

    MainWindow w;
    w.show();
    return a.exec();
}


// #include "mainwindow.h"
// #include "welcomescreen.h"

// #include <QApplication>






// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     //exampleUsage();
//     a.setWindowIcon(QIcon(":/images/codenheimer_icon3.png"));
//     int ret=MainWindow::firstTimeInit();
//     if(ret==-1){
//         return -1;
//     }
//     if(ret==1){
//         //launch onboarding flow
//         welcomeScreen wel;
//         wel.show();
//         return a.exec();
//     }
//     MainWindow w;
//     w.show();

//     return a.exec();
// }
