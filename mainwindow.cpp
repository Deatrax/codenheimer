#include "mainwindow.h"
#include "browselangwidget.h"
#include "browsetagwidget.h"
#include "qmenu.h"
#include "qpainter.h"
#include "snippetpreviewbox.h"
#include "snippetsettingspopup.h"
#include "tagadder.h"
#include "ui_mainwindow.h"
#include "predefines.h"
#include "editorwidget.h"
#include "searchsyetem.h"
#include <QDir>
#include <QSettings>



//Global and Static Space
QString MainWindow::company="AronoxStudios";
QString MainWindow::appName="Codenheimer";





//==================












MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadConfig();

    addTagtoList();
    //tagwidget
    //taglayout= new QVBoxLayout();
    //ui->tagWidget->setLayout(taglayout);
    //ui->tagWidget->layout()->addWidget(tagListWidget);
    //taglayout->addWidget(tagListWidget);
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

    ui->sandBox->clear();
    // ********Implementation using QGroupBox
    // for(int i=0;i<12;i++){
    //     snippetPreviewBox* pb=new snippetPreviewBox(this,this);
    //     pb->assignSnippet(mainStorage[i]);
    //     ui->sandBox->layout()->addWidget(pb);
    // }
    // QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    // ui->sandBox->layout()->addItem(verticalSpacer);
    //********

    //*********implementation using list widget
    //ui->sandBox->clear();


    ui->sandBox->setSpacing(3);
    ui->sandBox->setStyleSheet(
        "QListWidget::item {"
        "   border: 0px solid black;"
        // "   border-radius: 5px;"
        // "   padding: 5px;"
        // "   margin: 3px;"
        "}"
        "QListWidget::item:selected {"
        "   color: white;"
        "   border: 1px solid red;"
        "}"
        );


    for (auto& itr: filenameStorage) {
        snippetBaseClass*& snip = itr.second;

        // Create the custom widget
        snippetPreviewBox* pb = new snippetPreviewBox(this, this);
        pb->assignSnippet(snip);

        // Create a QListWidgetItem to hold the custom widget
        QListWidgetItem* item = new QListWidgetItem(ui->sandBox);

        // Set the size of the item to match the widget
        item->setSizeHint(pb->sizeHint());

        // Store snippetPreviewBox pointer inside Qt::UserRole
        item->setData(Qt::UserRole, QVariant::fromValue(pb));

        // Add the item to the list widget
        ui->sandBox->addItem(item);

        // Set the custom widget for this item (for display only)
        ui->sandBox->setItemWidget(item, pb);
    }



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
        ui->usernameAndMainSettingsButton->setText(QString(username.c_str())+"  ");

        //preparing the holders and other objects
        mainTagHolder=new tagHolder(tagCount);
        mainLangHolder=new langHolder(additionalTypeCount);
        clipboard = QApplication::clipboard(); // Get the clipboard object
        searchObj=new searchSystem;
        Julius= new cryptographicAgent();
        Julius->setHash(hashResult);
        readData();


        prepareCentralArea();
        searchPageSearchbar();
        prepareAddNewComboBox();
        prepareSettingsPage();


        prepareBrowsePage();



        //load complete, land on add new page
        ui->maincontentsStack->setCurrentIndex(0);
        //add to system tray
        createTrayActions();
        setClickableOptions(true);
        createSysTray();
        connect(trayIcon, &QSystemTrayIcon::activated, this,
                &MainWindow::iconActivated);
        if(trayEnabled) trayIcon->show();

    }


    void MainWindow::saveToSettings(const QString &username, const QString &hashResult, const QString &vault, int tag, int type) {
        // Step 1: Create or open the QSettings object
        QSettings settings(company, appName);

        // Step 2: Write data to settings
        settings.setValue("username", username);
        settings.setValue("hashres", hashResult);
        settings.setValue("vault", vault);
        settings.setValue("tag", tag);
        settings.setValue("type", type);
        settings.setValue("loginRun",false);
        settings.setValue("trayIcon",false);
    }

    int MainWindow::firstTimeInit()
    {
        std::ifstream inFile("firstrun.txt", std::ios::in); // Attempt to open the file in read mode

        if (!inFile.is_open()) { // Check if the file could not be opened and if not then this is the first time this is being run
            std::ofstream make("firstrun.txt");
            make.close();
            std::cerr << "Error: First time run checker File does not exist or cannot be opened!\n";
            std::string strn;
            char str[assist::PATH_SIZE];
            assist::getAppData_folder(str);
            std::string mode="wb";
            assist::ensure_directory_and_open_file(str,NULL,mode.c_str());


            mode="a";
            std::vector<std::string> filenames = {
                "tagDat.cdh",
                "langDat.cdh",
                "snipDatVault.cdh"
            };

            char filePathBuffer[assist::PATH_SIZE];

            for (const auto& filename : filenames) {
                std::strncpy(filePathBuffer, filename.c_str(), sizeof(filePathBuffer) - 1);
                filePathBuffer[sizeof(filePathBuffer) - 1] = '\0';

                assist::make_appData_filePath(filePathBuffer);

                if (assist::ensure_directory_and_open_file(str, filePathBuffer, mode.c_str())) {
                    QMessageBox::critical(nullptr, "Error", "Failed to initialize the application, please contact devs: ERR_NO_CREATE:"+QString().fromStdString(filename));
                    return -1;
                }
            }



            QSettings settings(company, appName);

            settings.setValue("username", "dummyUser");
            settings.setValue("hashres", "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8");
            settings.setValue("vault", "default");
            settings.setValue("tag",  6); //working on this
            settings.setValue("type", 0);
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

        ui->EditorsDefaultTabButton->setFont(CreteRoundFont);
        ui->defaultTabExplainer->setFont(CreteRoundFont);


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

        ui->addNewButton->setFont(centralElementsFont);
        ui->addNewButton->hide();

        centralElementsFont.setPointSize(13);
        ui->addNewLangDropdown->setFont(centralElementsFont);
    }

// <<<<<<< ryexocious-making-search-page
    void MainWindow::searchPageSearchbar(){
        //the useless dummy icon
        ui->searchDummyIcon->setText("");
        ui->searchDummyIcon->setIcon(QIcon(":/images/searchIcon.svg"));
        ui->searchDummyIcon->setIconSize(QSize(25, 25));

        //the search box font
        QFont searchBoxFont=CutiveMonoFont;
        searchBoxFont.setPointSize(26);
        ui->searchBoxLineEdit->setFont(searchBoxFont);

        //the snippet settings icon
        ui->snippetSettingsOnSearchPage->setText("");
        ui->snippetSettingsOnSearchPage->setIcon(QIcon(":/images/settingsIcon.svg"));
        ui->snippetSettingsOnSearchPage->setIconSize(QSize(23,23));

        ui->snippetPreviewBoxAreaOnSearchPage->setStyleSheet(
            "QListWidget{"
            "border:none;"
            "}"
            "QListWidget::item {"
            "   border: 0px solid black;"
            // "   border-radius: 5px;"
            // "   padding: 5px;"
            // "   margin: 3px;"
            "}"
            "QListWidget::item:selected {"
            "   color: white;"
            "   border: 1px solid red;"
            "}"
            );
    }

//     void MainWindow::readUconfig(){
//         char uconfigFile[500]="uconfig.cdh";
//         assist::make_appData_filePath(uconfigFile);
//         std::ifstream uconfigStream(uconfigFile, std::ios::in);
//         if(!uconfigStream.is_open()){
//             qDebug("Failed to open uconfig.cdh");
//             return;
// =======


    void MainWindow::prepareAddNewComboBox()
    {
        std::vector<string> langs=mainLangHolder->getLangList();
        for(auto& it:langs){
            ui->addNewLangDropdown->addItem(QString(it.c_str()));

        }
        ui->addNewLangDropdown->addItem("Select");
        ui->addNewLangDropdown->setCurrentText("Select");
        ui->addNewLangDropdown->setStyleSheet(
            "QComboBox {"
            "   qproperty-alignment: AlignCenter;"  // Centers text horizontally and vertically
            "   border:3px solid black;"
            "}"
            );
    }

    void MainWindow::prepareSettingsPage(){
        ui->settingsBackButton->setIcon(QIcon(":/images/backArrowIcon.svg"));
        ui->settingsBackButton->setIconSize(QSize(20,23));
        CreteRoundFont.setPointSize(23);
        ui->settingsTitle->setFont(CreteRoundFont);
        ui->generalSettingsTitle->setFont(CreteRoundFont);
        ui->snLSettingTitle->setFont(CreteRoundFont);
        ui->fileSettingsTitle->setFont(CreteRoundFont);
        CreteRoundFont.setPointSize(12);
        ui->usernameTitle->setFont(CreteRoundFont);
        ui->oldPasswordTitle->setFont(CreteRoundFont);
        ui->newPasswordTitle->setFont(CreteRoundFont);
        ui->usernameEdit->setFont(CreteRoundFont);
        ui->oldPasswordEdit->setFont(CreteRoundFont);
        ui->newPasswordEdit->setFont(CreteRoundFont);
        ui->vaultLocationTitle->setFont(CreteRoundFont);
        ui->vaultLocationEdit->setFont(CreteRoundFont);
        ui->sysTrayCheckBox->setCheckState( trayEnabled ?  Qt::Checked  : Qt::Unchecked);
         ui->OpenAtLoginCheckBox->setCheckState( loginEnabled ?  Qt::Checked  : Qt::Unchecked);

    }


    void MainWindow::readUconfig(){

        QSettings settings(company, appName);
        QString u=settings.value("username","default_user").toString();
        QString hs=settings.value("hashres","default_val").toString();
        QString va=settings.value("vault","default").toString();
        int ty=settings.value("type",0).toInt();
        int tg=settings.value("tag",0).toInt();
        bool login =settings.value("loginRun",false).toBool();
        bool tray =  settings.value("trayIcon",false).toBool();

        username=u.toStdString();
        hashResult=hs.toStdString();
        vaultLocation=va.toStdString();
        tagCount=tg;
        additionalTypeCount=ty;
        trayEnabled= tray;
        loginEnabled= login;



        qDebug() << "got from settings==" << u << hs << va << ty << tg;
        qDebug() << "got from settings==" << u.toStdString().c_str() << hs.toStdString().c_str() << va.toStdString().c_str() << ty << tg;
        qDebug("the stuff got from uconfig was:\nusername\t%s\nhashres\t%s\nvault\t%s\ntag\t%d\ntype\t%d\n",username.c_str(),hashResult.c_str(),vaultLocation.c_str(),tagCount,additionalTypeCount);

    }

    void MainWindow::readData(){
        char snippetVaultFile[assist::PATH_SIZE];
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
        lineNum=1;
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
            obj->innit(name,filename,lineNum,lang,tags,this);
            size_t lastDot = filename.find_last_of(".");
            std::string nameWithoutExt = (lastDot == std::string::npos) ? filename : filename.substr(0, lastDot);
            filenameStorage[nameWithoutExt] = obj;
            mainLangHolder->insert(obj);
            if(ifTags=="tags")mainTagHolder->insert(obj);
            // deprecated mainStorage.push_back(obj);
            //THIS IS WHERE JESSAN WILL ADD INSERT OF SEARCH CLASS
            searchObj->insert(name,obj);
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
            qDebug("Error: Tag not found for name: %s",tagName.c_str());
            passedName = "NoTag";
            passedColor = "";
        }
    }

    void MainWindow::copyToClipboard(const QString& text){
        
        //qDebug("copy to clipboard called");
        clipboard->setText(text); 
        if(QApplication::clipboard()->text() == text){
            ui->statusBar->showMessage("Copied to clipboard!", 2500); 
        } else {
            warnUser("Text copy to clipboard failed! Please contact devs to report bug");
        }
    }

    void MainWindow::addNewAction(){
        QString newName=ui->newSnippetNameBox->text();
        if(newName!=""){
            if(containsSpaces(newName)){
                warnUser("Please use name without spaces!!");
                return;
            }

            if(ui->addNewLangDropdown->currentText()!="Select")
            addNewSnippet(newName,ui->addNewLangDropdown->currentText());
            else warnUser("Please Select Language");
        }
        else warnUser("Please give a name");
        return;
    }

    void MainWindow::addNewSnippet(QString name, QString lang){
        qDebug()<<"Add new final reached: "<<name<<" "<<lang;

        //generate filename
        std::string filename =generateUniqueFilename(name,lang,1);
        if (filename == "stop") return;
        std::string filenameWithoutExt= filename;
        filename+=".cdh";
        snippetBaseClass* obj=generateSnippetObject(lang.toStdString());
        obj->innit( name.toStdString() , filename , lineNum , lang.toStdString() , std::vector<std::string>() ,this);
        mainLangHolder->insert(obj);
        filenameStorage[filenameWithoutExt]=obj;
        //Insert into search here
        searchObj->insert(name.toStdString(),obj);
        //=====updating the vault file
        std::string vaultDat=name.toStdString() + "," + filename + "," + lang.toStdString() + "," + "noTags";
        qDebug()<<"gonna write to vault file: "<<vaultDat;
        char vaultFilePath[assist::PATH_SIZE];
        std::strncpy(vaultFilePath, "snipDatVault.cdh", sizeof(vaultFilePath) - 1);
        vaultFilePath[sizeof(vaultFilePath) - 1] = '\0';
        assist::make_appData_filePath(vaultFilePath);
        if(assist::addLine(vaultFilePath,-1,vaultDat))
            showAutoCloseMessageBox(this,"Success!","Snippet added to vault success!");
        else{
            warnUser("Snippets failed to add in vault! \n Please check logs and contact devs");
            return;
        }
        //=============

        //======making the snippet code file
        obj->saveSnippetToFile("");

        openSnippetInEditor(obj,name,false);
    }


    ///
    /// \brief MainWindow::generateUniqueFilename a function for the careful and well considered generation of filename for snippets
    /// \param name the name of the snippet
    /// \param lang the lang of the snippet
    /// \param mode if mode =1, new snippet no obj exists yet, if mode =2, generating for renamed snippet
    /// \return the generated filename
    ///
    std::string MainWindow::generateUniqueFilename(const QString& name, const QString& lang, int mode, std::string oldFilename, snippetBaseClass *obj) {

        std::string filename = name.toStdString() + lang.toStdString();
        int i = 0;  // Start from 0 to check `name+".cdh"` first
        do {
            if (i == 3) {
                warnUser("This name has been used 3 times, please use another name");
                return "stop";
            }

            if (i > 0) {
                filename = name.toStdString() + lang.toStdString() + std::to_string(i);
            }

            i++;
        } while (filenameStorage.find(filename) != filenameStorage.end());

        if(mode==1){
            //filenameStorage[filename]=obj;
            return filename;
        }
        if(mode ==2){
            filenameStorage.erase(oldFilename);
            filenameStorage[filename]=obj;
            return filename;
        }
    }


    void MainWindow::showAutoCloseMessageBox(QWidget *parent,QString errTitle, QString msg) {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Information,
                                              errTitle,
                                              msg,
                                              QMessageBox::Ok,
                                              parent);

        msgBox->setAttribute(Qt::WA_DeleteOnClose);  // Delete after closing

        // Close the message box after 3000 ms (3 seconds)
        QTimer::singleShot(3000, msgBox, &QMessageBox::accept);

        msgBox->exec();
    }

    void MainWindow::warnUser(QString str)
    {

        // /*Using tooltip to warn user*/ QToolTip::showText(QCursor::pos(), str, nullptr, QRect(), 2000);
        showAutoCloseMessageBox(this, "Waring!!",str);
        qDebug()<<"User was warned: "<<str;
// >>>>>>> main
    }

    void MainWindow::closeTab()
    {
        //int idx = ui->editorTabs->indexOf(tab);
        // if (idx != -1) {  // Ensure the tab exists
        //     ui->editorTabs->removeTab(idx);
        // }

        // editorWidget* editor = qobject_cast<editorWidget*>(ui->editorTabs->widget(idx));
        // if (editor) {
        //     editor->close();
        // }

        qDebug() << "Tab count: " << ui->editorTabs->count();
        // ui->editorTabs->setCurrentIndex(0);
        // if (!ui->editorTabs) {
        //     qDebug() << "ERROR: ui->editorTabs is NULL!";
        //     return;
        // }
        // qDebug() << "Editor tabs pointer:" << ui->editorTabs;

        QWidget* currentTab = ui->editorTabs->currentWidget();
        if (!currentTab) {
            qDebug() << "No current tab selected!";
            return;
        }
        int currentIdx=ui->editorTabs->indexOf(currentTab);
        ui->editorTabs->removeTab(currentIdx);
        currentTab->close();

    }

    std::vector<string> MainWindow::getLangList(){
        return mainLangHolder->getLangList();
    }

    void MainWindow::openSnippetInEditor(snippetBaseClass* snipObj, QString& tabname, bool isOld)
    {
        editorWidget* newEditor=new editorWidget(this,this);
        newEditor->assign(snipObj,isOld);
        ui->editorTabs->addTab(newEditor,tabname);
        newEditor->tellIdx(ui->editorTabs->indexOf(newEditor));
        setMainIndex(2);
        ui->editorTabs->setCurrentWidget(newEditor);
        // ui->editorTabs->setCurrentIndex(ui->editorTabs->indexOf(newEditor));

        if(ui->editorTabs->currentWidget()==ui->defaultTab) {
            qDebug()<<"the current tab is default tab";
        }
        int i=ui->editorTabs->indexOf(ui->defaultTab);
        if(ui->editorTabs->isTabVisible(i)){
            ui->editorTabs->setTabVisible(i,false);
        }
    }


    bool MainWindow::deleteSnippet(snippetBaseClass* obj){

        if(obj->isLocked()){
            if( !Julius->authenticate() ){
                warnUser("Delete attempt stopped bacsue of wrong password!!");
                return false;
            }
        }

        
        //remove from tag holder
        if(mainTagHolder->removeSnippet(obj)) qDebug()<<"removed from tagHolder";
        else qDebug()<<"Snippet failed to remove from or didn't exist in tag holder";

        //remove from lang holder
        if(mainLangHolder->removeSnippet(obj)) qDebug()<<"snippet removed from lang holder";
        else qDebug()<<"snippet should have been in the lang holder";

        //remove from search trie
        searchObj->remove(obj);

        //remove from filehashmap
        if (filenameStorage.erase(obj->getOldFilename()))  qDebug() << "Filename removed from storage: " << QString::fromStdString(obj->getOldFilename());
        else qDebug() << "Filename not found in storage: " << QString::fromStdString(obj->getOldFilename());


        //delete from file
        if(obj->deleteFromVault())
            showAutoCloseMessageBox(this,"Success!","Snippet deleted from vault success!");
        else{
            warnUser("Snippets failed to delete from vault! \n Please check logs and contact devs");
            return false;
        }


        //delete the snippet itself that is call it's destructor
        delete obj;

        return true;
    }

    void MainWindow::renameSnippet(std::string newName, snippetBaseClass *obj)
    {
        //change name in lang holder
            //names are not stored here lol

        //change name in tag holder
            //names are not stored here lol

        //name was already changed in filename holder

        //change name in search trie
        searchObj->rename(newName , obj);
    }

    void MainWindow::snipetLangChanged( snippetBaseClass *obj, std::string lang)
    {
        mainLangHolder->removeSnippet(obj);
        mainLangHolder->insert(obj, lang);
    }

    void MainWindow::tagChanged(snippetBaseClass *obj)
    {
        mainTagHolder->removeSnippet(obj);
        mainTagHolder->insert(obj);
    }

    bool MainWindow::containsSpaces(QString& str) {
        return str.indexOf(' ') != -1; // indexOf returns -1 if no match is found
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


// <<<<<<< ryexocious-making-search-page
void MainWindow::on_searchBoxLineEdit_textChanged(const QString &arg1)
{
    ui->snippetPreviewBoxAreaOnSearchPage->clear();
    if(arg1!="" /*un comment this to see all*/ /*true*/) {
        std::vector<std::pair<std::string, std::vector<snippetBaseClass *>>> searchRet= searchObj->searchWithPrefix(arg1.toStdString());
        for (auto& itr : searchRet){
            for (auto& itr2 : itr.second) {
                // Create the custom widget
                snippetPreviewBox* pb = new snippetPreviewBox(this, this);
                pb->assignSnippet(itr2);

                // Create a QListWidgetItem to hold the custom widget
                QListWidgetItem* item = new QListWidgetItem(ui->snippetPreviewBoxAreaOnSearchPage);

                // Set the size of the item to match the widget
                item->setSizeHint(pb->sizeHint());

                // Store snippetPreviewBox pointer inside Qt::UserRole
                item->setData(Qt::UserRole, QVariant::fromValue(pb));

                // Add the item to the list widget
                ui->snippetPreviewBoxAreaOnSearchPage->addItem(item);

                // Set the custom widget for this item (for display only)
                ui->snippetPreviewBoxAreaOnSearchPage->setItemWidget(item, pb);
            }
        }
    }
    else return;
}

// =======
void MainWindow::on_newSnippetNameBox_textChanged(const QString &arg1)
{
    if(arg1!="") ui->addNewButton->show();
    else ui->addNewButton->hide();
}


void MainWindow::on_newSnippetNameBox_returnPressed()
{
    addNewAction();
}


void MainWindow::on_addNewButton_clicked()
{
    addNewAction();
}


void MainWindow::on_EditorsDefaultTabButton_clicked()
{
    setMainIndex(3);
}


void MainWindow::on_downarrow_clicked()
{
    sandBox();
    ui->maincontentsStack->setCurrentIndex(5);
}


void MainWindow::on_centralBrowseButton_clicked()
{
    setMainIndex(3);
// >>>>>>> main
}


void MainWindow::on_snippetSettingsTestButton_clicked()
{
    snippetSettingsPopup* pop=new snippetSettingsPopup(this);
    QListWidgetItem *selectedItem = ui->sandBox->currentItem();
    if (selectedItem) {
        QVariant data = selectedItem->data(Qt::UserRole);
        snippetPreviewBox *previewBox = data.value<snippetPreviewBox*>();

        if (previewBox) {
            // Do something with previewBox
            qDebug() << "Snippet Preview Box retrieved!";
            pop->assign(previewBox->getSnippetObj());
            pop->show();
        } else {
            qDebug() << "No snippetPreviewBox associated with this item.";
            delete pop;
        }
    } else {
        qDebug() << "No item selected.";
        delete pop;
    }



}

std::vector<std::string> MainWindow::getTagList(){
    return mainTagHolder->getTagList();
}

void MainWindow::on_snippetSettingsOnSearchPage_clicked()
{
    snippetSettingsPopup* pop=new snippetSettingsPopup(this);
    QListWidgetItem *selectedItem = ui->snippetPreviewBoxAreaOnSearchPage->currentItem();
    if (selectedItem) {
        QVariant data = selectedItem->data(Qt::UserRole);
        snippetPreviewBox *previewBox = data.value<snippetPreviewBox*>();

        if (previewBox) {
            // Do something with previewBox
            qDebug() << "Snippet Preview Box retrieved!";
            pop->assign(previewBox->getSnippetObj());
            pop->show();
        } else {
            qDebug() << "No snippetPreviewBox associated with this item.";
            delete pop;
        }
    } else {
        ui->statusBar->showMessage("Please select snippet first!", 2500);
        delete pop;
    }
}

//settingspage functions




void MainWindow::on_addTagButton_clicked()
{
    tagAdder *addtag = new tagAdder(this,this);
    addtag->setWindowTitle("Add Tag Wizard");
    addtag->setWindowFlags(Qt::Window);// | Qt::CustomizeWindowHint | Qt::WindowTitleHint

    //addtag->setAttribute(Qt::WA_DeleteOnClose);
    addtag->show();

}

void MainWindow::addTagtoList()//tagViewer *tag)
{
    ui->tagListWidget->clear();

    qDebug() <<"total tags found" <<getTagList().size();

    for(auto &tname: getTagList())
    {
        QListWidgetItem *newItem= new QListWidgetItem(ui->tagListWidget);
        string name,color;
        getTagInfo(tname,name,color);
        tagViewer *tag=new tagViewer(this);
        tag->setTag(name,color);



        newItem->setSizeHint(tag->sizeHint());
        ui->tagListWidget->addItem(newItem);
        ui->tagListWidget->setItemWidget(newItem,tag);
        //ui->tagListWidget->setItemWidget();
    }
}

void MainWindow::getMainTagHolder(const std::string &tagName, const std::string &tagColor )//need to be changed later
{
    mainTagHolder->addTag(tagName,tagColor) ;
    tagCount++;
    QSettings settings(company, appName);
    settings.setValue("tag",tagCount);
}

//-------------------system tray related functions------------------


void MainWindow::createTrayActions(){
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, [this]
            (){
                this->hide();
                setClickableOptions(false);
            });
    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this,
            &QWidget::showMaximized);
    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, [this]()
            {
                this->showNormal();  // Restore the window
                setClickableOptions(true);  // Call
                setClickableOptions(true);
            });
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp,
            &QCoreApplication::quit);
}

void MainWindow::createSysTray(){
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    QString menuStyle =
        "QMenu {"
        "    background-color: #fff;"
        "    border: 1px solid #e2e8f0;"
        "    border-radius: 5px;"
        "    font-family: 'Basier circle', -apple-system, system-ui, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, 'Noto Sans', sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol', 'Noto Color Emoji';"
    "    font-size: 15px;"
    "    font-weight: 600;"
    "    padding: 5px;"
    "}"
    "QMenu::item {"
    "    background-color: #fff;"
    "    color: #0d172a;"
    "    padding: 5px 10px;"
    "    border-radius: 5px;"
    "    margin: 5px 0;"
        //Codenheimer © Aronox Studios 52
        "}"
        "QMenu::item:selected {"
        "    background-color: #221D23;"
        "    color: #fff;"
        "}";
    trayIconMenu->setStyleSheet(menuStyle);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/images/codenheimer_icon3.png"));
    QPixmap pixmap(":/images/codenheimer_icon3.png");
    QPainter painter(&pixmap);
    painter.setPen(Qt::white); // Or any color that contrasts with the background
        painter.drawText(pixmap.rect().adjusted(20, 0, -20, 0),
                         Qt::AlignRight, "Text");
    painter.end();
    trayIcon->setIcon(QIcon(pixmap));
}

void MainWindow::setClickableOptions(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(!visible);
}
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        qDebug("single click detected");
        //Codenheimer © Aronox Studios 53
            break;
    case QSystemTrayIcon::DoubleClick:
        qDebug("dounel click detedted");
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug("middle click detedted");
        break;
    default:
        ;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    setClickableOptions(false);
    if (!event->spontaneous() || !isVisible())
        return;
    if (trayIcon->isVisible()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Systray"));
        msgBox.setText(tr("The program will keep running in the " "system tray. To terminate the program, " "choose <b>Quit</b> in the context menu " "of the system tray entry."));
        //msgBox.setStyleSheet(messageboxStyle);
        msgBox.exec();
        hide();
        event->ignore();
        return;
    }
    trayIcon->hide();
    completeDeletes();
    event->accept();
    //Codenheimer © Aronox Studios 54
}
void MainWindow::trayVisibility(bool flag){
    flag? trayIcon->show() : trayIcon->hide();
}

void MainWindow::on_sysTrayCheckBox_clicked(bool checked)
{
    if(checked!= trayEnabled){

        if(checked==false){
            trayEnabled=checked;
            trayVisibility(false);
            QSettings settings(company, appName);
            settings.setValue("trayIcon",trayEnabled);
            qDebug()<<"tray enabled is now false";

        }
        else{
            trayEnabled=checked;
            QSettings settings(company, appName);
            settings.setValue("trayIcon",trayEnabled);
            trayVisibility(true);
            qDebug()<<"tray enabled is now true";
        }
    }
    else if(checked == trayEnabled) return;
}

//end of system tray functions

//------Open on Login Functions---------

void MainWindow::setAutoStartWindows(bool flag) {
#ifdef _WIN32
    QString appName = "Codenheimer";
    QString appPath =
        QDir::toNativeSeparators(QCoreApplication::applicationFilePath
                                 ());
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                       QSettings::NativeFormat);
    if (flag) {
        settings.setValue(appName, appPath);
    } else {
        settings.remove(appName);
    }
#elif defined(__APPLE__) // macOS specific code
    QString appName = "TestApp";
    QString appPath = QCoreApplication::applicationFilePath();
    QString plistPath = QDir::homePath() + "/Library/LaunchAgents/" + appName + ".plist";
    QFile plistFile(plistPath);

    if (flag) {
        if (plistFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&plistFile);
            out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            out << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
            out << "<plist version=\"1.0\">\n";
            out << "<dict>\n";
            out << "    <key>Label</key>\n";
            out << "    <string>" << appName << "</string>\n";
            out << "    <key>ProgramArguments</key>\n";
            out << "    <array>\n";
            out << "        <string>" << appPath << "</string>\n";
            out << "    </array>\n";
            out << "    <key>RunAtLoad</key>\n";
            out << "    <true/>\n";
            out << "</dict>\n";
            out << "</plist>\n";
            plistFile.close();
        }
    } else {
        if (plistFile.exists()) {
            plistFile.remove();
        }
    }
#endif
    //Codenheimer © Aronox Studios

}

void MainWindow::on_OpenAtLoginCheckBox_clicked(bool checked)
{
    if(checked!=loginEnabled){
        setAutoStartWindows(checked);
        //preparing for getting the configs file
        // char file[260]="uconfig.spenc";
        // make_appData_filePath(file);
        //open and change the file
        if(checked==false){
            loginEnabled=false;
            QSettings settings(company, appName);
            settings.setValue("loginRun",loginEnabled);
            qDebug()<<"tray enabled is now false";
        }
        else{
            loginEnabled=true;
            QSettings settings(company, appName);
            settings.setValue("loginRun",loginEnabled);
            qDebug()<<"tray enabled is now false";
        }
    }
}


void MainWindow::on_removeTagButton_clicked()
{
    QListWidgetItem *selectedItem = ui->tagListWidget->currentItem();
    if (!selectedItem) {
        qDebug() << "No tag selected for removal.";
        return;
    }

    // Retrieve the tag widget from the list item
    tagViewer *tag = qobject_cast<tagViewer *>(ui->tagListWidget->itemWidget(selectedItem));
    if (!tag) {
        qDebug() << "Error: Selected item does not contain a tagViewer.";
        return;
    }

    QString tagName = tag->getTagName();
    qDebug() << "Removing tag:" << tagName;
    mainTagHolder->removeTag(tagName.toStdString());

    delete selectedItem;

    qDebug() << "Tag removed successfully.";
}








void MainWindow::on_testCryptoButton_clicked()
{
    // encryptText();
}

void MainWindow::encryptText(QString file, QString data)
{
    // QString password = "password" /*ui->OnlyFilePassword->text()*/;
    // if (password.isEmpty()) {
    //     QMessageBox::warning(this, "Error", "Please enter a password.");
    //     return;
    // }
    // QString fileName = "testFILE" /*ui->Filename->text()*/;
    // if (fileName.isEmpty()) {
    //     QMessageBox::warning(this, "Error", "Please enter a file name.");
    //     return;
    // }
    // QString filePath = QFileDialog::getSaveFileName(this, "Save Encrypted File", QDir::homePath() + "/" + fileName);
    // if (filePath.isEmpty()) {
    //     return; // User canceled the save dialog
    // }

    Julius->encryptToFile(file , data );
}



void MainWindow::on_testDecryptButton_clicked()
{
    // decryptText();
}


QString MainWindow::decryptText(QString fileName)
{
    // QString password = "password" /*ui->OnlyFilePassword->text()*/;
    // if (password.isEmpty()) {
    //     QMessageBox::warning(this, "Error", "Please enter a password.");
    //     return;
    // }
    // QString filePath = QFileDialog::getOpenFileName(this, "Select Encrypted File", QDir::homePath());
    // if (filePath.isEmpty()) {
    //     return; // User canceled the file dialog
    // }

    return Julius->decryptFromFile( fileName );
}

void MainWindow::test(){
    qDebug()<<"this is some test text\n";
}





void MainWindow::on_pushButton_2_clicked()
{

}






// Schedule deletion (add to map and save to temp file)
void MainWindow::scheduleDeletion(int lineNumber, const QString &filePath) {
    pendingDeletions[lineNumber] = filePath;
    qDebug()<<"the snippet has been scheduled for removal from the file";
    savePendingDeletes();
}

// Save pending deletions to temp file
void MainWindow::savePendingDeletes() {
    QFile file(tempFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto &[lineNumber, filePath] : pendingDeletions) {
            out << lineNumber << "|" << filePath << "\n";
        }
        file.close();
    }
    qDebug()<<"the snippet has been added to temp file for deletetion";
}

// 
/**
 * @brief Load pending deletions from temp file (on startup)
 * this is not being used right now because the are more bounds 
 * checking needed to be done. like for example, 
 *  when loading, the loading part needs to be done before the actual data loading
 * also when the data load will happpend the ones scheduled for deletion will also 
 * appear. this is not intended behaviour. in which case a crash detection mechanism needs to be 
 * in place that will check for undeleted snippets, meaning thea application crashed last time a\
 * and then will detelte them, inform the user and then restart the application
 */
void MainWindow::loadPendingDeletes() {
    QFile file(tempFilePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split("|");
            if (parts.size() == 2) {
                int lineNumber = parts[0].toInt();
                QString filePath = parts[1];
                pendingDeletions[lineNumber] = filePath;
            }
        }
        file.close();
    }
}

// Delete lines and corresponding snippet files before closing
void MainWindow::completeDeletes() {
    //QString vaultFilePath = "path/to/snipDatVault.cdh";  // Update with actual path

    char vaultFilePath[assist::PATH_SIZE];
    std::strncpy(vaultFilePath, "snipDatVault.cdh", sizeof(vaultFilePath) - 1);
    vaultFilePath[sizeof(vaultFilePath) - 1] = '\0';
    assist::make_appData_filePath(vaultFilePath);

    for (const auto &[lineNumber, filePath] : pendingDeletions) {
        assist::removeLine(vaultFilePath, lineNumber);

        if (QFile::remove(filePath)) {
            qDebug() << "Snippet file deleted: " << filePath;
        } else {
            qDebug() << "Failed to delete: " << filePath;
        }
    }

    pendingDeletions.clear();
    QFile::remove(tempFilePath);  // Remove temp file after processing
}




void MainWindow::applyFontToChildren(QWidget* parent, const QFont& font) {
    if (!parent) return;

    parent->setFont(font);  // Apply font to the parent itself

    for (QObject* child : parent->children()) {
        QWidget* childWidget = qobject_cast<QWidget*>(child);
        if (childWidget) {
            childWidget->setFont(font);  // Apply font to child widget
            applyFontToChildren(childWidget, font);  // Recursively apply to grandchildren
        }
    }
}

void MainWindow::prepareBrowsePage(){
    ui->browseShortCutButton->setIcon(QIcon(":/images/rightArowIcon.svg"));
    ui->browseShortCutButton->setIconSize(QSize(26,30));

    ui->browseBackButton->setIcon(QIcon(":/images/backArrowIcon.svg"));
    ui->browseBackButton->setIconSize(QSize(20,23));

    QFont brosweFont=CreteRoundFont;
    brosweFont.setPointSize(14);

    QFont browseFont2= CreteRoundFont;
    browseFont2.setPointSize(18);

    applyFontToChildren(ui->browseViewFilterBox, brosweFont);

    ui->browseMainTitle->setFont(browseFont2);
    ui->tagsLabel->setFont(browseFont2);
    ui->browseViewTitle->setFont(browseFont2);
    ui->filterTitle->setFont(browseFont2);

    //preparing the langs
    browseLangFL= new FlowLayout(10,20,8);
    std::vector<std::string> langs=mainLangHolder->getLangList();
    for (auto &it : langs) {
        browseLangWidget *bl=new browseLangWidget(this, this);
        filterWidget *fw=new filterWidget(this, bl);
        bl->init(it, fw);
        fw->init(it, 1);
        browseLangFL->addWidget(bl);

        // Create a QListWidgetItem to hold the custom widget
                QListWidgetItem* item = new QListWidgetItem(ui->filderLangList);

        // Set the size of the item to match the widget
        item->setSizeHint(fw->sizeHint());

        // Store snippetPreviewBox pointer inside Qt::UserRole
        item->setData(Qt::UserRole, QVariant::fromValue(fw));

        // Add the item to the list widget
        ui->filderLangList->addItem(item);

        // Set the custom widget for this item (for display only)
        ui->filderLangList->setItemWidget(item, fw);
    }
    ui->langAreaBox->setLayout(browseLangFL);


    //preparing the tags
    qDebug()<<"==============================================starting to add the tags";
    browseTagFL= new FlowLayout(10, 10, 8);
    std::vector<std::string> tagssss= getTagList();
    for (auto &it : tagssss) {
        qDebug()<<"adding the tag: "<<it;
        browseTagWidget *bt=new browseTagWidget(this ,this);
        filterWidget *fw=new filterWidget(this, bt);
        bt->init(it, fw);
        fw->init(it, 2);
        browseTagFL->addWidget(bt);

        // Create a QListWidgetItem to hold the custom widget
        QListWidgetItem* item = new QListWidgetItem(ui->filterTagList);

        // Set the size of the item to match the widget
        item->setSizeHint(fw->sizeHint());

        // Store snippetPreviewBox pointer inside Qt::UserRole
        item->setData(Qt::UserRole, QVariant::fromValue(fw));

        // Add the item to the list widget
        ui->filterTagList->addItem(item);

        // Set the custom widget for this item (for display only)
        ui->filterTagList->setItemWidget(item, fw);
    }
    ui->tagAreaBox->setLayout(browseTagFL);
    qDebug()<<"==============================================tag add complete";

    ui->browsePageStack->setCurrentIndex(0);
}

void MainWindow::on_browseShortCutButton_clicked()
{
    ui->browsePageStack->setCurrentIndex(1);
    updateBrowseView();
}


void MainWindow::on_browseBackButton_clicked()
{
    ui->browsePageStack->setCurrentIndex(0);
}


std::vector<snippetBaseClass*> MainWindow::getFilteredSnippets(
    const std::vector<std::string>& langFilters,
    const std::vector<std::string>& tagFilters,
    const std::vector<snippetBaseClass*>& snippets,
    langHolder* langDB,  // Used for checking snippet-language mapping
    tagHolder* tagDB     // Used for checking snippet-tag mapping
    ) {
    std::vector<snippetBaseClass*> filteredSnippets;

    for (auto* snippet : snippets) {
        bool matchesLang = false, matchesTag = false;

        // Check if snippet matches any language filter
        for (const auto& lang : langFilters) {
            if (langDB->snippetExistsInLang(lang, snippet)) {
                matchesLang = true;
                break;  // No need to check further
            }
        }

        // Check if snippet matches any tag filter
        for (const auto& tag : tagFilters) {
            if (tagDB->snippetExistsInTag(tag, snippet)) {
                matchesTag = true;
                break;  // No need to check further
            }
        }

        // If both conditions are met, add to result vector
        if (matchesLang || matchesTag) {
            filteredSnippets.push_back(snippet);
        }
    }

    return filteredSnippets;
}


void MainWindow::updateBrowseView(){

    // ui->browseMainViewArea->clear();
    ui->browseMainViewArea->clear();

    int limit=ui->perPageSee->value();

    std::vector<std::pair<std::string, std::vector<snippetBaseClass *>>> searchRet= searchObj->pagedSearch(limit, false);
    std::vector<snippetBaseClass*> snippetVector;

    for (const auto& pair : searchRet) {
        snippetVector.insert(snippetVector.end(), pair.second.begin(), pair.second.end());
    }

    std::vector<snippetBaseClass*> finalList;
    if(langFilters.size() >0 || tagFilters.size() >0 )
        finalList= getFilteredSnippets(langFilters, tagFilters, snippetVector, mainLangHolder, mainTagHolder);
    else
        finalList= snippetVector;
    // //std::vector<std::pair<std::string, std::vector<snippetBaseClass *>>> searchRet= searchObj->searchWithPrefix("");
    // for (auto& itr : searchRet){
    //         for (auto& itr2 : itr.second) {
    //             // Create the custom widget
    //             snippetPreviewBox* pb = new snippetPreviewBox(this, this);
    //             pb->assignSnippet(itr2);

    //             // Create a QListWidgetItem to hold the custom widget
    //             QListWidgetItem* item = new QListWidgetItem(ui->browseMainViewArea);

    //             // Set the size of the item to match the widget
    //             item->setSizeHint(pb->sizeHint());

    //             // Store snippetPreviewBox pointer inside Qt::UserRole
    //             item->setData(Qt::UserRole, QVariant::fromValue(pb));

    //             // Add the item to the list widget
    //             ui->browseMainViewArea->addItem(item);

    //             // Set the custom widget for this item (for display only)
    //             ui->browseMainViewArea->setItemWidget(item, pb);
    //         }
    //     }



    for (auto& itr2 : finalList) {
        // Create the custom widget
        snippetPreviewBox* pb = new snippetPreviewBox(this, this);
        pb->assignSnippet(itr2);

        // Create a QListWidgetItem to hold the custom widget
        QListWidgetItem* item = new QListWidgetItem(ui->browseMainViewArea);

        // Set the size of the item to match the widget
        item->setSizeHint(pb->sizeHint());

        // Store snippetPreviewBox pointer inside Qt::UserRole
        item->setData(Qt::UserRole, QVariant::fromValue(pb));

        // Add the item to the list widget
        ui->browseMainViewArea->addItem(item);

        // Set the custom widget for this item (for display only)
        ui->browseMainViewArea->setItemWidget(item, pb);
    }


}


void MainWindow::updateBrowseView(bool flag){

    // // ui->browseMainViewArea->clear();
    // ui->browseMainViewArea->clear();

    // int limit=ui->perPageSee->value();

    // std::vector<std::pair<std::string, std::vector<snippetBaseClass *>>> searchRet= searchObj->pagedSearch(limit, flag);


    // //std::vector<std::pair<std::string, std::vector<snippetBaseClass *>>> searchRet= searchObj->searchWithPrefix("");
    // for (auto& itr : searchRet){
    //     for (auto& itr2 : itr.second) {
    //         // Create the custom widget
    //         snippetPreviewBox* pb = new snippetPreviewBox(this, this);
    //         pb->assignSnippet(itr2);

    //         // Create a QListWidgetItem to hold the custom widget
    //         QListWidgetItem* item = new QListWidgetItem(ui->browseMainViewArea);

    //         // Set the size of the item to match the widget
    //         item->setSizeHint(pb->sizeHint());

    //         // Store snippetPreviewBox pointer inside Qt::UserRole
    //         item->setData(Qt::UserRole, QVariant::fromValue(pb));

    //         // Add the item to the list widget
    //         ui->browseMainViewArea->addItem(item);

    //         // Set the custom widget for this item (for display only)
    //         ui->browseMainViewArea->setItemWidget(item, pb);
    //     }
    // }

    // ui->browseMainViewArea->clear();
    ui->browseMainViewArea->clear();

    int limit=ui->perPageSee->value();

    std::vector<std::pair<std::string, std::vector<snippetBaseClass *>>> searchRet= searchObj->pagedSearch(limit, flag);
    std::vector<snippetBaseClass*> snippetVector;

    for (const auto& pair : searchRet) {
        snippetVector.insert(snippetVector.end(), pair.second.begin(), pair.second.end());
    }

    std::vector<snippetBaseClass*> finalList;
    if(langFilters.size() >0 || tagFilters.size() >0 )
        finalList= getFilteredSnippets(langFilters, tagFilters, snippetVector, mainLangHolder, mainTagHolder);
    else
        finalList= snippetVector;
    // //std::vector<std::pair<std::string, std::vector<snippetBaseClass *>>> searchRet= searchObj->searchWithPrefix("");
    // for (auto& itr : searchRet){
    //         for (auto& itr2 : itr.second) {
    //             // Create the custom widget
    //             snippetPreviewBox* pb = new snippetPreviewBox(this, this);
    //             pb->assignSnippet(itr2);

    //             // Create a QListWidgetItem to hold the custom widget
    //             QListWidgetItem* item = new QListWidgetItem(ui->browseMainViewArea);

    //             // Set the size of the item to match the widget
    //             item->setSizeHint(pb->sizeHint());

    //             // Store snippetPreviewBox pointer inside Qt::UserRole
    //             item->setData(Qt::UserRole, QVariant::fromValue(pb));

    //             // Add the item to the list widget
    //             ui->browseMainViewArea->addItem(item);

    //             // Set the custom widget for this item (for display only)
    //             ui->browseMainViewArea->setItemWidget(item, pb);
    //         }
    //     }



    for (auto& itr2 : finalList) {
        // Create the custom widget
        snippetPreviewBox* pb = new snippetPreviewBox(this, this);
        pb->assignSnippet(itr2);

        // Create a QListWidgetItem to hold the custom widget
        QListWidgetItem* item = new QListWidgetItem(ui->browseMainViewArea);

        // Set the size of the item to match the widget
        item->setSizeHint(pb->sizeHint());

        // Store snippetPreviewBox pointer inside Qt::UserRole
        item->setData(Qt::UserRole, QVariant::fromValue(pb));

        // Add the item to the list widget
        ui->browseMainViewArea->addItem(item);

        // Set the custom widget for this item (for display only)
        ui->browseMainViewArea->setItemWidget(item, pb);
    }


}


void MainWindow::on_perPageSee_valueChanged(int arg1)
{
    updateBrowseView();
}


void MainWindow::on_nextPageButton_clicked()
{
    updateBrowseView(true);
}


void MainWindow::on_previousPageButton_clicked()
{
    updateBrowseView();
}


void MainWindow::applyFilter(std::string text, int type){
    if(type == 1){
        langFilters.push_back(text);
    }
    else if (type == 2){
        tagFilters.push_back(text);
    }

    updateBrowseView();
    ui->browsePageStack->setCurrentIndex(1);
}


void MainWindow::removeFilter(std::string text, int type){
    if(type == 1){
        langFilters.erase( std::find(langFilters.begin() , langFilters.end() , text) );
    }
    else if (type == 2){
        tagFilters.erase( std::find(langFilters.begin() , langFilters.end() , text) );;
    }

    updateBrowseView();
    ui->browsePageStack->setCurrentIndex(1);
}





void MainWindow::on_mofobutton_clicked()
{
    qDebug()<<"the button was clicked";
    hashResult=Julius->setPassFirstTime();
    qDebug()<<"the hash result is: "<<hashResult;
    QSettings settings(company, appName);
    settings.setValue("hashres", QString(hashResult.c_str() ) );
}

