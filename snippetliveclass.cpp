#include "snippetliveclass.h"
#include "mainwindow.h"

snippetLiveClass::snippetLiveClass() {}

snippetLiveClass::~snippetLiveClass()
{
    //nothing to delete coz no dynamic memory was allocated  here

    SNIPPET.clear();
    name.clear();
    filename.clear();
    lang.clear();
}

void snippetLiveClass::innit(std::string nam, std::string filenam, int linNum, std::string lng, std::vector<std::string> tgs, QMainWindow *MM)
    // : name(nam)
    // , filename(filenam)
    // , lineNum(linNum)
    // , lang(lng)
    // , tags(tgs)
{
    name=nam;
    filename=filenam;
    lineNum=linNum;
    lang=lng;
    tags=tgs;
    masterWindow=MM;
    MainWindow* mm=static_cast<MainWindow*>(masterWindow);
    if (mm) {
        mm->test();  // Now you can call MainWindow-specific functions
    }

    size_t pos = filename.find_last_of('.');

    if (pos != std::string::npos) {
        // Extract the extension
        std::string extension = filename.substr(pos + 1);
        qDebug("got the file extension: %s",extension.c_str());
        // Perform actions based on the extension
        if (extension == "cdh") {
            isLockedVar=false;
        } else if (extension == "scdh") {
            isLockedVar=true;
        }
    }


    std::cout << "Snippet Initialized: " << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Filename: " << filename << std::endl;
    std::cout << "Line Number: " << lineNum << std::endl;
    std::cout << "Language: " << lang << std::endl;
    std::cout << "Locked Status: " << isLockedVar << std::endl;

    std::cout << "Tags: ";
    if (tags.empty()) {
        std::cout << "No tags" << std::endl;
    } else {
        for (const auto& tag : tags) {
            std::cout << tag << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::string>* snippetLiveClass::getInfo(std::string& nam, std::string& lng)
{
    nam=name;
    lng=lang;
    return &tags;
}

std::string snippetLiveClass::getSnippet()
{
    if (!SNIPPET.empty()) {
        return SNIPPET;
    }
    else if( isLocked() ){
        MainWindow* mainW=static_cast<MainWindow*>(masterWindow);
        if (mainW) {
            return mainW->decryptText(filename.c_str()).toStdString();  // Now you can call MainWindow-specific functions
        }
    }
    else {
        char snippetCodeFile[assist::PATH_SIZE];
        // for now considering vaultLocation is default
        if (true /*vaultLocation == "default"*/) {
            qDebug("the vault location is default");
            std::strncpy(snippetCodeFile, filename.c_str(), sizeof(snippetCodeFile) - 1);
            snippetCodeFile[sizeof(snippetCodeFile) - 1] = '\0';
            assist::make_appData_filePath(snippetCodeFile);
        } else {
            // Implement other vault location handling later
        }

        std::ifstream file(snippetCodeFile, std::ios::in);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();
        SNIPPET = buffer.str();
        return SNIPPET;
    }
}

std::vector<std::string> snippetLiveClass::getTags()
{
    return tags;
}

bool search(const std::string &str,   std::vector<snippetBaseClass*> ret){
    return true;
}

// void snippetLiveClass::setEditor(QTextDocument *)
// {

// }

std::string snippetLiveClass::getLang() {
    std::cerr<<"get lang in live class was called"<<std::endl;
    return lang;
}

bool snippetLiveClass::isCustom()
{
    return false;
}

void snippetLiveClass::putColors(std::unordered_map<std::string, std::vector<std::string>> &colorsData)
{
    return;
}

void snippetLiveClass::putTags(std::vector<std::string> &foundTags)
{
    tags=foundTags;
}

bool snippetLiveClass::isLocked()
{
    return isLockedVar;
}

// <<<<<<< ryexocious-making-search-page
//search option
void snippetLiveClass::insert(const std::string &str, snippetBaseClass *targ){
    //absolutely nothing;
}

bool snippetLiveClass::search(const std::string& str, std::vector<snippetBaseClass*>& ret)
{
    return false;
}

std::vector<std::pair<std::string, std::vector<snippetBaseClass *>>> snippetLiveClass::searchWithPrefix(const std::string &prefix)
{
    return std::vector<std::pair<std::string, std::vector<snippetBaseClass *>>>();
}
// =======


std::string& snippetLiveClass::EditSnippet()
{
    if (!SNIPPET.empty()) {
        return SNIPPET;
    } else {
        char snippetCodeFile[assist::PATH_SIZE];
        // for now considering vaultLocation is default
        if (true /*vaultLocation == "default"*/) {
            qDebug("the vault location is default");
            std::strncpy(snippetCodeFile, filename.c_str(), sizeof(snippetCodeFile) - 1);
            snippetCodeFile[sizeof(snippetCodeFile) - 1] = '\0';
            assist::make_appData_filePath(snippetCodeFile);
        } else {
            // Implement other vault location handling later
        }

        std::ifstream file(snippetCodeFile, std::ios::in);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();
        SNIPPET = buffer.str();
        return SNIPPET;
    }
}

bool snippetLiveClass::saveSnippetToFile(string snippet)
{
    char snippetCodeFile[assist::PATH_SIZE];

    // Assuming vaultLocation is "default" for now
    if (true /*vaultLocation == "default"*/) {
        qDebug("The vault location is default");
        std::strncpy(snippetCodeFile, filename.c_str(), sizeof(snippetCodeFile) - 1);
        snippetCodeFile[sizeof(snippetCodeFile) - 1] = '\0';
        assist::make_appData_filePath(snippetCodeFile);
    } else {
        // Implement other vault location handling later
    }

    // Open file for writing
    std::ofstream file(snippetCodeFile, std::ios::out | std::ios::trunc);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
        assist::errLog(std::string("Failed to open file for writing: " + filename).c_str());
        return false;
    }

    file << snippet;
    file.close();


    return true;
}

bool snippetLiveClass::updateSnippetDetails(std::string nam,std::string filenam, std::vector<std::string> tgs, std::string lng, bool lock ){


    if (lock) {
        filenam+=".scdh";
    }
    else{
        filenam+=".cdh";
    }
    if(updateSnippetFilename(filenam) ) filename=filenam;

    tags.clear();
    tags=tgs;
    if(name!=nam) name=nam;
    if(lang!=lng) lang=lng;


    std::string vaultDat=name+","+filename+","+lang+",";
    if(tags.size()>0){
        vaultDat+="tags";
        for ( auto& tag : tags) {
            vaultDat+=","+tag;
        }
    }
    else vaultDat+="noTags";

    qDebug()<<"gonna write to vault file: "<<vaultDat;
    char vaultFilePath[assist::PATH_SIZE];
    std::strncpy(vaultFilePath, "snipDatVault.cdh", sizeof(vaultFilePath) - 1);
    vaultFilePath[sizeof(vaultFilePath) - 1] = '\0';
    assist::make_appData_filePath(vaultFilePath);
    /*return*/ assist::editLine(vaultFilePath,lineNum,vaultDat);


    if( isLockedVar != lock) updateFileSecurity(lock);
}

bool snippetLiveClass::updateSnippetFilename(std::string newFilename) {
    if(filename==newFilename){
        return false;
    }
    // Attempt to rename the file
    char snippetCodeFileOld[assist::PATH_SIZE];
    if (true /*vaultLocation == "default"*/) {
        qDebug("The vault location is default");
        std::strncpy(snippetCodeFileOld, filename.c_str(), sizeof(snippetCodeFileOld) - 1);
        snippetCodeFileOld[sizeof(snippetCodeFileOld) - 1] = '\0';
        assist::make_appData_filePath(snippetCodeFileOld);
    } else {
        // Implement other vault location handling later
    }


    char snippetCodeFileNew[assist::PATH_SIZE];
    if (true /*vaultLocation == "default"*/) {
        qDebug("The vault location is default");
        std::strncpy(snippetCodeFileNew, newFilename.c_str(), sizeof(snippetCodeFileNew) - 1);
        snippetCodeFileNew[sizeof(snippetCodeFileNew) - 1] = '\0';
        assist::make_appData_filePath(snippetCodeFileNew);
    } else {
        // Implement other vault location handling later
    }



    std::string oldFilePath = snippetCodeFileOld;
    std::string newFilePath = snippetCodeFileNew;

    if (std::rename(oldFilePath.c_str(), newFilePath.c_str()) != 0) {
        qDebug() << "Error renaming file: " << strerror(errno);
        return false;
    }
    qDebug()<<"the code file name was updated successfully";
    return true;
}

void snippetLiveClass::updateFileSecurity(bool lock)
{
    MainWindow* mainW=static_cast<MainWindow*>(masterWindow);
    if (mainW) {
        if(lock){
            mainW->encryptText( QString( filename.c_str() ) , QString( getSnippet().c_str() ) );
        }

        else if(!lock){
            SNIPPET = mainW->decryptText( QString( filename.c_str() ) ).toStdString();
            saveSnippetToFile(SNIPPET);
        }
    }
    isLockedVar = lock;
}

std::string snippetLiveClass::getOldFilename(){
    size_t lastDot = filename.find_last_of(".");
    std::string nameWithoutExt = (lastDot == std::string::npos) ? filename : filename.substr(0, lastDot);
    return nameWithoutExt;
}

bool snippetLiveClass::deleteFromVault()
{
    // char vaultFilePath[assist::PATH_SIZE];
    // std::strncpy(vaultFilePath, "snipDatVault.cdh", sizeof(vaultFilePath) - 1);
    // vaultFilePath[sizeof(vaultFilePath) - 1] = '\0';
    // assist::make_appData_filePath(vaultFilePath);


    //if(assist::removeLine(vaultFilePath,lineNum)){

        char snippetCodeFileOld[assist::PATH_SIZE];
        if (true /*vaultLocation == "default"*/) {
            qDebug("The vault location is default");
            std::strncpy(snippetCodeFileOld, filename.c_str(), sizeof(snippetCodeFileOld) - 1);
            snippetCodeFileOld[sizeof(snippetCodeFileOld) - 1] = '\0';
            assist::make_appData_filePath(snippetCodeFileOld);

            // if (std::remove(snippetCodeFileOld) == 0) {
                // qDebug() << "File deleted successfully.\n";
            // } else {
                // std::perror("Error deleting file");
                // return false;
            // }
            MainWindow* mainW=static_cast<MainWindow*>(masterWindow);
            if (mainW) {
                mainW->scheduleDeletion(lineNum, QString(snippetCodeFileOld));
            }

            
            return true;
        } else {
            // Implement other vault location handling later
        }
    // }
    // else{
    //     return false;
    // }
}

bool snippetLiveClass::remove(snippetBaseClass *obj)
{
    return false;
}
