#ifndef BROWSETAGWIDGET_H
#define BROWSETAGWIDGET_H

#include "mainwindow.h"
#include <QWidget>

namespace Ui {
class browseTagWidget;
}

class browseTagWidget : public QWidget
{
    Q_OBJECT

public:
    explicit browseTagWidget(MainWindow *mainwindow,QWidget *parent = nullptr);
    ~browseTagWidget();

    void init(std::string tag);
private slots:
    void on_pushButton_clicked();

private:
    Ui::browseTagWidget *ui;
    MainWindow *masterWindow;
    QString thisTag;
    QFont thisFont;
};

#endif // BROWSETAGWIDGET_H
