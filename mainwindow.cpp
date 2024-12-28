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
    qDebug("===========================Main window destructor was called");
    delete mainLangHolder;
    delete mainTagHolder;
}

//START OF ADDITIONAL NON-SLOT BASED  FUNCTIONS

/**
 * @brief sandBox
 * this function is mainly designed as an experimentation area for the ui or just any thing really
 */
void MainWindow::sandBox(){
    for(int i=0;i<6;i++){
        snippetPreviewBox* pb=new snippetPreviewBox(this,this);
        pb->assignSnippet(mainStorage[i]);
        ui->sandBox->layout()->addWidget(pb);
    }
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->sandBox->layout()->addItem(verticalSpacer);

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

        readUconfig();

        mainTagHolder=new tagHolder(tagCount);
        mainLangHolder=new langHolder(additionalTypeCount);

        readData();

        //testing holders
        mainLangHolder->testPrintCustomLang("typeName5");
        mainLangHolder->testPrintCustomLang("typeName4");


        //testing the snippet previewBox
        // for(int i=0;i<5;i++){
        //     snippetPreviewBox* snp=new snippetPreviewBox(this);
        //     ui->testbox->layout()->addWidget(snp);
        // }

        prepareCentralArea();

        sandBox();
        //load complete, land on add new page
        ui->maincontentsStack->setCurrentIndex(0);
    }

    

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
        char uconfigFile[500]="uconfig.cdh";
        assist::make_appData_filePath(uconfigFile);
        std::ifstream uconfigStream(uconfigFile, std::ios::in);
        if(!uconfigStream.is_open()){
            qDebug("Failed to open uconfig.cdh");
            return;
        }
        std::getline(uconfigStream,username);
        std::getline(uconfigStream,hashResult);
        std::getline(uconfigStream,vaultLocation);

        std::string tagC, typeC;
        std::getline(uconfigStream,tagC);
        std::getline(uconfigStream,typeC);
        tagCount=std::stoi(tagC);
        additionalTypeCount=std::stoi(typeC);
        qDebug("the stuff got from uconfig was:\nusername\t%s\nhashres\t%s\nvault\t%s\ntag\t%d\ntype\t%d\n",username.c_str(),hashResult.c_str(),vaultLocation.c_str(),tagCount,additionalTypeCount);
    }

    void MainWindow::readData(){
        char snippetVaultFile[500];
        if(vaultLocation=="default"){
            qDebug("the vault location is default");
            std::strncpy(snippetVaultFile, "snipDatVault.cdh", sizeof(snippetVaultFile) - 1);
            snippetVaultFile[sizeof(snippetVaultFile) - 1] = '\0';
            assist::make_appData_filePath(snippetVaultFile);
        }else{
            std::strncpy(snippetVaultFile, vaultLocation.c_str(), sizeof(snippetVaultFile) - 1);
            snippetVaultFile[sizeof(snippetVaultFile) - 1] = '\0';
        }


        std::string lineStore;
        std::ifstream snippetVaultStream(snippetVaultFile,std::ios::in);
        int lineNum=1;
        while(std::getline(snippetVaultStream,lineStore)){
            string ifTags,name,filename,lang,tag;
            std::vector<std::string> tags;
            std::stringstream ss(lineStore);
            std::string lockStat;
            getline(ss,name,',');
            getline(ss,filename,',');
            getline(ss,lang,',');

            getline(ss,ifTags,',');
            if (ifTags == "tags") {

                // Read the remaining part of the line for tags
                while (std::getline(ss, tag, ',')) {
                    tags.push_back(tag);
                }
            }
            snippetBaseClass* obj=generateSnippetObject(lang);
            obj->innit(name,filename,lineNum,lang,tags);
            mainLangHolder->insert(obj);
            if(ifTags=="tags")mainTagHolder->insert(obj);
            mainStorage.push_back(obj);
            //THIS IS WHERE JESSAN WILL ADD INSERT OF SEARCH CLASS

            // Output or use the tags for testing
            std::cout << "Name: " << name << ", Filename: " << filename
                      << ", Lang: " << lang << ", Tags: ";
            if(ifTags=="tags"){
                for (const auto& t : tags) {
                    std::cout << t << " ";
                }
            }

            std::cout << std::endl;
            lineNum++;
        }
    }

    snippetBaseClass* MainWindow::generateSnippetObject(std::string lang){
        snippetBaseClass* obj;
        if(lang=="c"){
            obj=new snippetC;
            return obj;
        }
        if(lang=="cpp"){
            obj=new snippetCPP;
            return obj;
        }
        if(lang=="css"){
            obj=new snippetCSS;
            return obj;
        }
        if(lang=="java"){
            obj=new snippetJAVA;
            return obj;
        }
        if(lang=="py"){
            obj=new snippetPY;
            return obj;
        }
        else{
            obj=new snippetCustom;
            return obj;
        }
    }


    void MainWindow::getTagInfo(string tagName, std::string &passedName, std::string& passedColor){
        // mainTagHolder->getTagInfo(tagName,passedName,passedColor);
        auto it = (*mainTagHolder)[tagName]; // Check if tagName exists in the map
        if (it != nullptr) {
            passedName = it->tagName;
            passedColor = it->tagColor;
        } else {
            qDebug("Error: Tag not found for name ");
            passedName = "NoTag";
            passedColor = "";
        }
    }



//END OF ADDITIONAL NON-SLOT BASED FUNCTIONS

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

