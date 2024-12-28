#include "snippetpreviewbox.h"
#include "ui_snippetpreviewbox.h"
#include "tagviewer.h"

snippetPreviewBox::snippetPreviewBox(MainWindow *mainwindow, QWidget *parent)
    :QWidget(parent),
    ui(new Ui::snippetPreviewBox),
    masterWindow(mainwindow)
{
    ui->setupUi(this);

    //setting the icon
    QPixmap pixmap(":/images/lockIcon.svg");
    ui->lockIcon->setPixmap(pixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    //getting and setting the font
    CutiveMonoFont=masterWindow->getFont("Cutive");
    CutiveMonoFont.setPointSize(19);
    ui->Name->setFont(CutiveMonoFont);
}

snippetPreviewBox::~snippetPreviewBox()
{
    delete ui;
}

void snippetPreviewBox::assignSnippet(snippetBaseClass* snippet){
    assignedSnippet=snippet;
    tags=assignedSnippet->getInfo(name, lang);
    ui->Name->setText(QString::fromStdString(name));
    ui->langLabel->setText(QString::fromStdString(lang));
    setTags();
}

void snippetPreviewBox::setTags(){

    for(auto& str: *tags/*int i=0;i<6;i++*/){
        std::string name="dickpbfdb";
        std::string color="red";
        masterWindow->getTagInfo(str, name, color);
        if(name=="NoTag") continue;
        tagViewer* tv=new tagViewer(this);
        tv->setTag(name, color);
        ui->tagsBox->layout()->addWidget(tv);
    }
}
