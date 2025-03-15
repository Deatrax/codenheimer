#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QMainWindow>
#include <QCryptographicHash>
#include <QFontDatabase>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QSettings>

namespace Ui {
class welcomeScreen;
}

class welcomeScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit welcomeScreen(QWidget *parent = nullptr);
    ~welcomeScreen();

private slots:
    void on_startButton_clicked();

    void on_showPasswordButton_pressed();

    void on_showPasswordButton_released();

    void on_userUpdateButton_clicked();

private:
    Ui::welcomeScreen *ui;
    QFont CutiveMonoFont;
    QFont CreteRoundFont;
    void applyFontToChildren(QWidget *parent, const QFont &font);
};

#endif // WELCOMESCREEN_H
