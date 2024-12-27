#include "snippetpreviewbox.h"
#include "ui_snippetpreviewbox.h"

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
    assignedSnippet->getInfo(name, lang, tags);
    ui->Name->setText(QString::fromStdString(name));
}
