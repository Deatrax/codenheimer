#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <predefines.h>
#include <QFontDatabase>

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
private slots:
    void on_sidebarButton_clicked();

    void on_AddnewSidebarButton_clicked();

    void on_SearchSidebarButton_clicked();

    void on_EditorsSidebarButton_clicked();

    void on_BrowseSidebarButton_clicked();

    void on_usernameAndMainSettingsButton_clicked();

private:
    Ui::MainWindow *ui;
    void loadConfig();


protected:
    QFont CutiveMonoFont;
    QFont CreteRoundFont;
    void loadCustomFonts();
    void centreSidebarButtons();
    void setSidebarButtonIcons();
    void prepareCentralArea();
};
#endif // MAINWINDOW_H
