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

    void on_page2nxt_clicked();

    void on_page3nxt_clicked();

    void on_page3prev_clicked();

    void on_page4nxt_clicked();

    void on_page4prev_clicked();

    void on_page5nxt_clicked();

    void on_page5prev_clicked();

    void on_page6nxt_clicked();

    void on_page6prev_clicked();

    void on_page7nxt_clicked();

    void on_page7prev_clicked();

    void on_page8nxt_clicked();

    void on_page8prev_clicked();

    void on_page9nxt_clicked();

    void on_page9prev_clicked();

    void on_finishWelcomeButton_clicked();

private:
    Ui::welcomeScreen *ui;
    QFont CutiveMonoFont;
    QFont CreteRoundFont;
    void applyFontToChildren(QWidget *parent, const QFont &font);
    void applyNavigationButtonIcons();
};

#endif // WELCOMESCREEN_H
