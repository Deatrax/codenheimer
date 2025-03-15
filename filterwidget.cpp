#include "filterwidget.h"
#include "ui_filterwidget.h"

filterWidget::filterWidget(MainWindow *mainwindow, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::filterWidget)
    , masterWindow(mainwindow)
{
    ui->setupUi(this);
}

void filterWidget::init(std::string text, int type){
    thisText=text;
    filterType=type;

    ui->checkBox->setText(QString(thisText.c_str()) );

}

void filterWidget::activated(){
    if( !ui->checkBox->isChecked())
    {
        ui->checkBox->setCheckState(Qt::Checked);
    }
}

filterWidget::~filterWidget()
{
    delete ui;
}

void filterWidget::on_checkBox_stateChanged(int arg1)
{
    arg1 ? masterWindow->applyFilter(thisText, filterType) : masterWindow->removeFilter(thisText, filterType);
}

