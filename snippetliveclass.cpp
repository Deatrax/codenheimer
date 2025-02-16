#include "snippetliveclass.h"

snippetLiveClass::snippetLiveClass() {}

void snippetLiveClass::innit(std::string nam, std::string filenam, int linNum, std::string lng, std::vector<std::string> tgs)
{
    name=nam;
    filename=filenam;
    lineNum=linNum;
    lang=lng;
    tags=tgs;

    size_t pos = filename.find_last_of('.');

    if (pos != std::string::npos) {
        // Extract the extension
        std::string extension = filename.substr(pos + 1);
        qDebug("got the file extension: %s",extension.c_str());
        // Perform actions based on the extension
        if (extension == "scdh") {
            isLockedVar=false;
        } else if (extension == "pcdh") {
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

std::vector<std::string> snippetLiveClass::getTags()
{
    return tags;
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

string &snippetLiveClass::EditSnippet()
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

bool snippetLiveClass::updateSnippetDetails(){

    std::string vaultDat=name+","+filename+","+lang+",";
    if(tags.size()>0){
        vaultDat+="tags";
        for ( auto& tag : tags) {
            vaultDat+=tag;
        }
    }
    else vaultDat+="noTags";

    qDebug()<<"gonna write to vault file: "<<vaultDat;
    char vaultFilePath[assist::PATH_SIZE];
    std::strncpy(vaultFilePath, "snipDatVault.cdh", sizeof(vaultFilePath) - 1);
    vaultFilePath[sizeof(vaultFilePath) - 1] = '\0';
    assist::make_appData_filePath(vaultFilePath);
    return assist::editLine(vaultFilePath,lineNum,vaultDat);
}
