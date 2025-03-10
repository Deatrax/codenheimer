#ifndef BROWSELANGWIDGET_H
#define BROWSELANGWIDGET_H

#include "filterwidget.h"
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

    void init(std::string lang, filterWidget *fw);
private slots:
    void on_pushButton_clicked();

private:
    Ui::browseLangWidget *ui;
    QString thisLang;
    MainWindow *masterWindow;
    QFont thisFont;
    filterWidget *thisFilterWidget;
};

#endif // BROWSELANGWIDGET_H
