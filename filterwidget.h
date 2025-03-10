#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include "mainwindow.h"
#include <QWidget>

namespace Ui {
class filterWidget;
}

class filterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit filterWidget(MainWindow *mainwindow, QWidget *parent = nullptr);
    ~filterWidget();

    void init(std::string text, int type);
    void activated();
private slots:
    void on_checkBox_stateChanged(int arg1);

private:
    Ui::filterWidget *ui;
    MainWindow *masterWindow;
    std::string thisText;
    int filterType;
};

#endif // FILTERWIDGET_H
