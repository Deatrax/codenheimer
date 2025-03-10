#include "browselangwidget.h"
#include "ui_browselangwidget.h"

browseLangWidget::browseLangWidget(MainWindow *mainwindow, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::browseLangWidget)
    , masterWindow(mainwindow)
{
    ui->setupUi(this);
}

browseLangWidget::~browseLangWidget()
{
    delete ui;
}

void browseLangWidget::init(std::string lang){
    thisLang = QString(lang.c_str());

    thisFont= masterWindow->getFont("Cutive");
    thisFont.setPointSize(18);
    ui->pushButton->setFont(thisFont);
    ui->pushButton->update();
    ui->pushButton->setText(thisLang);
}

void browseLangWidget::on_pushButton_clicked()
{

}

