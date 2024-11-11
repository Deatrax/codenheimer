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

private slots:
    void on_sidebarButton_clicked();

private:
    Ui::MainWindow *ui;
    void loadConfig();

protected:
    QFont CutiveMonoFont;
    QFont CreteRoundFont;
    void loadCustomFonts();
    void centreSidebarButtons();
    void setSidebarButtonIcons();
};
#endif // MAINWINDOW_H
