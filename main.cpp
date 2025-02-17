#include "mainwindow.h"

#include <QApplication>
#include <QSettings>


void saveToSettings(const QString &username, const QString &hashResult, const QString &vault, int tag, int type) {
    // Step 1: Create or open the QSettings object
    QSettings settings("AronoxStudios", "Codenheimer");

    // Step 2: Write data to settings
    settings.setValue("username", username);
    settings.setValue("hashres", hashResult);
    settings.setValue("vault", vault);
    settings.setValue("tag", tag);
    settings.setValue("type", type);
}

void exampleUsage() {
    QString username = "user123";
    QString hashResult = "hashed_result_string";
    QString vault = "default";
    int tag = 6;
    int type = 6;

    saveToSettings(username, hashResult, vault, tag, type);
    qDebug() << "Data saved to settings!";
}

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
