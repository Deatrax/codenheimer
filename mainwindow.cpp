#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
}


    void MainWindow::loadConfig(){
        setWindowTitle("Codenheimer");
        ui->sidebarButton->setIcon(QIcon(":/images/sidebarButtonSVG.svg"));
        ui->sidebarButton->setIconSize(QSize(30, 33));
        loadCustomFonts();
        ui->topbarTitle->setFont(newFont);
    }

    void MainWindow::loadCustomFonts(){
        int fontId = QFontDatabase::addApplicationFont(":/fonts/CutiveMono-Regular.ttf");
        if (fontId != -1) {
            QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
            newFont=QFont(family);
            newFont.setPointSize(36);
        } else {
            qDebug() << "Font loading failed!";
        }
    }

    void MainWindow::on_sidebarButton_clicked()
    {
        if(!ui->sidebar->isHidden()) ui->sidebar->hide();
        else ui->sidebar->show();
    }

