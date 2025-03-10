#ifndef BROWSETAGWIDGET_H
#define BROWSETAGWIDGET_H

#include "filterwidget.h"
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

    void init(std::string tag, filterWidget *fw);
private slots:
    void on_pushButton_clicked();

private:
    Ui::browseTagWidget *ui;
    MainWindow *masterWindow;
    QString thisTag;
    QFont thisFont;
    filterWidget *thisFilterWidget;
};

#endif // BROWSETAGWIDGET_H
