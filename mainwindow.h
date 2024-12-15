#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <predefines.h>
#include <QFontDatabase>
#include "helperFunctions.h"
#include <fstream>
#include <iostream>
#include <sstream>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QFont getFont(std::string str);
    void setMainIndex(int n);
    void readData();
    static int firstTimeInit();

private slots:
    void on_sidebarButton_clicked();

    void on_AddnewSidebarButton_clicked();

    void on_SearchSidebarButton_clicked();

    void on_EditorsSidebarButton_clicked();

    void on_BrowseSidebarButton_clicked();

    void on_usernameAndMainSettingsButton_clicked();

    void on_centralSearchIcon_clicked();

    void on_centralSearchBoxLE_returnPressed();

private:
    Ui::MainWindow *ui;
    void loadConfig();




    char coreUserConfig[1000];
protected:
    QFont CutiveMonoFont;
    QFont CreteRoundFont;
    void loadCustomFonts();
    void centreSidebarButtons();
    void setSidebarButtonIcons();
    void prepareCentralArea();

};
#endif // MAINWINDOW_H
