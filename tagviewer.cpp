#include "tagviewer.h"
#include "ui_tagviewer.h"

tagViewer::tagViewer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::tagViewer)
{
    ui->setupUi(this);
}

tagViewer::~tagViewer()
{
    delete ui;
}


void tagViewer::setTag(std::string Name, std::string Color){
    tagName = QString::fromStdString(Name);
    tagColor = QString::fromStdString(Color);
    ui->tagLabel->setText("#"+tagName);
    QString style=  "background:" + tagColor + ";" + "border-radius:7px; padding-left:5px; padding-right:5px;";
    ui->tagLabel->setStyleSheet(style);
}

QString tagViewer::getTagName(){
    return tagName;
}
