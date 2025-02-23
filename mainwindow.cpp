#include "mainwindow.h"
#include "snippetpreviewbox.h"
#include "snippetsettingspopup.h"
#include "ui_mainwindow.h"
#include "predefines.h"
#include "editorwidget.h"
#include "searchsyetem.h"
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

        //preparing the holders
        mainTagHolder=new tagHolder(tagCount);
        mainLangHolder=new langHolder(additionalTypeCount);
        clipboard = QApplication::clipboard(); // Get the clipboard object
        searchObj=new searchSystem;
        readData();

        //testing holders
        // mainLangHolder->testPrintCustomLang("typeName5");
        // mainLangHolder->testPrintCustomLang("typeName4");


        //testing the snippet previewBox
        // for(int i=0;i<5;i++){
        //     snippetPreviewBox* snp=new snippetPreviewBox(this);
        //     ui->testbox->layout()->addWidget(snp);
        // }

        //custom UI styling calls
        prepareCentralArea();
//<<<<<<< ryexocious-making-search-page
        searchPageSearchbar();

//=======
        prepareAddNewComboBox();
//>>>>>>> main



        //testing the editor
        // editorWidget *widget=new editorWidget(this);
        //ui->editorsPage->layout()->addWidget(widget);
        // ui->defaultTab->layout()->addWidget(widget);



        //load complete, land on add new page
        ui->maincontentsStack->setCurrentIndex(0);
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
            settings.setValue("hashres", "noHash");
            settings.setValue("vault", "default");
            settings.setValue("tag",  0);
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


    void MainWindow::readUconfig(){

        // //OLD system using File IO============
        // char uconfigFile[assist::PATH_SIZE]="uconfig.cdh";
        // assist::make_appData_filePath(uconfigFile);
        // std::ifstream uconfigStream(uconfigFile, std::ios::in);
        // if(!uconfigStream.is_open()){
        //     qDebug("Failed to open uconfig.cdh");
        //     return;
        // }
        //
        // std::getline(uconfigStream,username);
        // std::getline(uconfigStream,hashResult);
        // std::getline(uconfigStream,vaultLocation);
        // std::string tagC, typeC;
        //
        // std::getline(uconfigStream,tagC);
        // std::getline(uconfigStream,typeC);
        // tagCount=std::stoi(tagC);
        // additionalTypeCount=std::stoi(typeC);
        // qDebug("the stuff got from uconfig was:\nusername\t%s\nhashres\t%s\nvault\t%s\ntag\t%d\ntype\t%d\n",username.c_str(),hashResult.c_str(),vaultLocation.c_str(),tagCount,additionalTypeCount);
        //========================================

        //Qsettings system
        QSettings settings(company, appName);

        QString u=settings.value("username","default_user").toString();
        QString hs=settings.value("hashres","default_val").toString();
        QString va=settings.value("vault","default").toString();
        int ty=settings.value("type",6).toInt();
        int tg=settings.value("tag",6).toInt();

        username=u.toStdString();
        hashResult=hs.toStdString();
        vaultLocation=va.toStdString();
        tagCount=tg;
        additionalTypeCount=ty;


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
            obj->innit(name,filename,lineNum,lang,tags);
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
        obj->innit(name.toStdString(),filename,lineNum,lang.toStdString(),std::vector<std::string>());
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
        newEditor->assign(snipObj,false);
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


    void MainWindow::deleteSnippet(snippetBaseClass* obj){
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
            return;
        }


        //delete the snippet itself that is call it's destructor
        delete obj;
    }

    void MainWindow::renameSnippet(snippetBaseClass *obj)
    {
        //change name in lang holder
            //names are not stored here lol

        //change name in tag holder
            //names are not stored here lol

        //name was already changed in filename holder

        //change name in search trie
        searchObj->rename(obj);
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

