#include "browsetagwidget.h"
#include "ui_browsetagwidget.h"

browseTagWidget::browseTagWidget(MainWindow *mainwindow,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::browseTagWidget)
    , masterWindow(mainwindow)
{
    ui->setupUi(this);
}

browseTagWidget::~browseTagWidget()
{
    delete ui;
}

void browseTagWidget::init(std::string tag, filterWidget *fw){
    thisTag = QString(tag.c_str());

    thisFont= masterWindow->getFont("Cutive");
    thisFont.setPointSize(18);
    ui->pushButton->setFont(thisFont);
    ui->pushButton->setText("#"+thisTag);

    thisFilterWidget = fw;
}

void browseTagWidget::on_pushButton_clicked()
{
    thisFilterWidget->activated();
}

