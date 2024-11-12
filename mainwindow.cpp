#include "mainwindow.h"
#include "snippetpreviewbox.h"
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
        ui->topbarTitle->setFont(CutiveMonoFont);
        ui->sidebarButtons->setFont(CreteRoundFont); //set the font of the sidebar page change buttons
        ui->usernameAndMainSettingsButton->setFont(CreteRoundFont); //set the same font for the settings button
        ui->sidebar->layout()->setAlignment(ui->usernameAndMainSettingsButton, Qt::AlignHCenter); //align the settings button at the centre
        centreSidebarButtons();
        setSidebarButtonIcons();


        //testing the snipper previewBox
        for(int i=0;i<5;i++){
            snippetPreviewBox* snp=new snippetPreviewBox(this);
            ui->testbox->layout()->addWidget(snp);
        }


        ui->maincontentsStack->setCurrentIndex(0);
    }

    void MainWindow::loadCustomFonts(){
        int fontId = QFontDatabase::addApplicationFont(":/fonts/CutiveMono-Regular.ttf");
        if (fontId != -1) {
            QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
            CutiveMonoFont=QFont(family);
            CutiveMonoFont.setPointSize(36);
        } else {
            qDebug() << "Font loading failed!";
        }


        int fontId2 = QFontDatabase::addApplicationFont(":/fonts/CreteRound-Regular.ttf");
        if (fontId2 != -1) {
            QString family = QFontDatabase::applicationFontFamilies(fontId2).at(0);
            CreteRoundFont=QFont(family);
            CreteRoundFont.setPointSize(14);
        } else {
            qDebug() << "Font loading failed!";
        }
    }

    ///centering the sidebar buttons
    void MainWindow::centreSidebarButtons(){
        // Get the layout from the UI file
        QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->sidebarButtons->layout());

        if (layout) {
            // Iterate through each item in the layout and set alignment to center horizontally
            for (int i = 0; i < layout->count(); ++i) {
                QWidget *widget = layout->itemAt(i)->widget();
                if (widget) {
                    layout->setAlignment(widget, Qt::AlignHCenter); // Align each widget horizontally center
                }
            }
        }
    }

    ///assigning the icons of the sidebar buttons
    void MainWindow::setSidebarButtonIcons(){
        ui->EditorsSidebarButton->setFont(CreteRoundFont);
        ui->AddnewSidebarButton->setFont(CreteRoundFont);
        ui->SearchSidebarButton->setFont(CreteRoundFont);
        ui->BrowseSidebarButton->setFont(CreteRoundFont);


        // search button
        ui->SearchSidebarButton->setIcon(QIcon(":/images/searchIcon.svg"));
        ui->SearchSidebarButton->setIconSize(QSize(18, 18)); // Adjust icon size as needed
        ui->SearchSidebarButton->setLayoutDirection(Qt::RightToLeft); // Puts the icon on the right side

        // editors button
        ui->EditorsSidebarButton->setIcon(QIcon(":/images/editorIcon.svg"));
        ui->EditorsSidebarButton->setIconSize(QSize(19, 19));
        ui->EditorsSidebarButton->setLayoutDirection(Qt::RightToLeft);

        // browse button
        ui->downarrow->setIcon(QIcon(":/images/downArrowIcon.svg"));
        ui->downarrow->setIconSize(QSize(17, 7));
        ui->downarrow->setLayoutDirection(Qt::RightToLeft);

        // username And Main Settings Button button
        ui->usernameAndMainSettingsButton->setIcon(QIcon(":/images/settingsIcon.svg"));
        ui->usernameAndMainSettingsButton->setIconSize(QSize(21, 21));
        ui->usernameAndMainSettingsButton->setLayoutDirection(Qt::RightToLeft);
    }

    QFont MainWindow::getFont(std::string str){
        if(str=="Cutive") return CutiveMonoFont;
        else if(str=="Crete") return CreteRoundFont;
    }

    void MainWindow::on_sidebarButton_clicked()
    {
        if(!ui->sidebar->isHidden()) ui->sidebar->hide();
        else ui->sidebar->show();
    }


void MainWindow::on_AddnewSidebarButton_clicked()
{
    ui->maincontentsStack->setCurrentIndex(0);
}


void MainWindow::on_SearchSidebarButton_clicked()
{
    ui->maincontentsStack->setCurrentIndex(1);
}


void MainWindow::on_EditorsSidebarButton_clicked()
{
    ui->maincontentsStack->setCurrentIndex(2);
}


void MainWindow::on_BrowseSidebarButton_clicked()
{
    ui->maincontentsStack->setCurrentIndex(3);
}


void MainWindow::on_usernameAndMainSettingsButton_clicked()
{
    ui->maincontentsStack->setCurrentIndex(4);
}

