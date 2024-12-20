#include "mainwindow.h"
#include "snippetpreviewbox.h"
#include "ui_mainwindow.h"
#include "predefines.h"

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

//START OF HUMAN ADDED FUNCTIONS

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

        //testing holders
        tagHolder tg(5);
        langHolder langs(5);
        langs.testPrintCustomLang("typeName5");
        langs.testPrintCustomLang("typeName4");
        qDebug("the size of tagHolder:%d ",sizeof(tg));
        qDebug("the size of langHolder:%d ",sizeof(langs));
        qDebug("the size of this:%d ",sizeof(*this));

        readUconfig();
        readData();

        //testing the snippet previewBox
        // for(int i=0;i<5;i++){
        //     snippetPreviewBox* snp=new snippetPreviewBox(this);
        //     ui->testbox->layout()->addWidget(snp);
        // }

        prepareCentralArea();


        //load complete, land on add new page
        ui->maincontentsStack->setCurrentIndex(0);
    }

    // void MainWindow::readData(){

    // }

    int MainWindow::firstTimeInit()
    {

        std::ifstream inFile("firstrun.txt", std::ios::in); // Attempt to open the file in read mode

        if (!inFile.is_open()) { // Check if the file could not be opened and if not then this is the first time this is being run
            std::ofstream make("firstrun.txt");
            make.close();
            std::cerr << "Error: File does not exist or cannot be opened!\n";
            std::string strn;
            char str[500];
            assist::getAppData_folder(str);
            std::string mode="wb";
            assist::ensure_directory_and_open_file(str,NULL,mode.c_str());
            return 1;
            //makeSampleFile();
        }
        else return 0;
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

    ///Sets the current page as specified and sets the button shading correspondingly
    void MainWindow::setMainIndex(int n){
        int x=ui->maincontentsStack->currentIndex();
        if(x==n) return;
        ui->maincontentsStack->setCurrentIndex(n);
        switch (n) {
        case 0:
            ui->AddnewSidebarButton->setStyleSheet(styles::buttonVisited);
            break;
        case 1:
            ui->SearchSidebarButton->setStyleSheet(styles::buttonVisited);
            break;
        case 2:
            ui->EditorsSidebarButton->setStyleSheet(styles::buttonVisited);
            break;
        case 3:
            ui->BrowseSidebarButton->setStyleSheet(styles::buttonBrowseVisited);
            break;
        default:
            break;
        }

        switch (x) {
        case 0:
            ui->AddnewSidebarButton->setStyleSheet(styles::buttonNormal);
            break;
        case 1:
            ui->SearchSidebarButton->setStyleSheet(styles::buttonNormal);
            break;
        case 2:
            ui->EditorsSidebarButton->setStyleSheet(styles::buttonNormal);
            break;
        case 3:
            ui->BrowseSidebarButton->setStyleSheet(styles::buttonBrowseNormal);
            break;
        default:
            break;
        }
    }

    //setting the font and font size of the central elements in the add new page
    void MainWindow::prepareCentralArea(){

        //new snippet box
        QFont centralElementsFont=CutiveMonoFont;
        centralElementsFont.setPointSize(20);
        ui->newSnippetNameBox->setFont(centralElementsFont);

        //search box
        ui->centralSearchBoxLE->setFont(centralElementsFont);
        /*QPixmap pixmap(":/images/searchIcon.svg");
        ui->centralSearchIcon->setPixmap(pixmap.scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation))*/;
        ui->centralSearchIcon->setIcon(QIcon(":/images/searchIcon.svg"));
        ui->centralSearchIcon->setIconSize(QSize(25, 25));

        //browse box
        ui->centralBrowseButton->setFont(centralElementsFont);
        ui->centralBrowseButton->setIcon(QIcon(":images/centralBrowseIcon.svg"));
        ui->centralBrowseButton->setIconSize(QSize(32, 28));
    }

    void MainWindow::readUconfig(){

    }

    void MainWindow::readData(){
        //data read args

    }


//END OF HUMAN ADDED FUNCTIONS

void MainWindow::on_sidebarButton_clicked()
{
    if(!ui->sidebar->isHidden()) ui->sidebar->hide();
    else ui->sidebar->show();
}


void MainWindow::on_AddnewSidebarButton_clicked()
{
    setMainIndex(0);
}


void MainWindow::on_SearchSidebarButton_clicked()
{
    setMainIndex(1);
}


void MainWindow::on_EditorsSidebarButton_clicked()
{
    setMainIndex(2);
}


void MainWindow::on_BrowseSidebarButton_clicked()
{
    setMainIndex(3);
}


void MainWindow::on_usernameAndMainSettingsButton_clicked()
{
    setMainIndex(4);
}


void MainWindow::on_centralSearchIcon_clicked()
{

}


void MainWindow::on_centralSearchBoxLE_returnPressed()
{

}

