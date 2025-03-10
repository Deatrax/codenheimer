#ifndef BROWSELANGWIDGET_H
#define BROWSELANGWIDGET_H

#include "mainwindow.h"
#include <QWidget>

namespace Ui {
class browseLangWidget;
}

class browseLangWidget : public QWidget
{
    Q_OBJECT

public:
    explicit browseLangWidget(MainWindow *mainwindow, QWidget *parent = nullptr);
    ~browseLangWidget();

    void init(std::string lang);
private slots:
    void on_pushButton_clicked();

private:
    Ui::browseLangWidget *ui;
    QString thisLang;
    MainWindow *masterWindow;
    QFont thisFont;
};

#endif // BROWSELANGWIDGET_H
