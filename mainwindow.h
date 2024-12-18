#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <predefines.h>
#include <QFontDatabase>
#include "helperFunctions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "snippetbaseclass.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class typeHolder{
    private:
        struct type{
            std::string typ;
            std::vector<snippetBaseClass*> snippets;
        };

        std::unordered_map<std::string, type>typeToSnippets;




};


class tagHolder{

    private:
        struct tag{
            bool hasData=false;
            std::string tagName;
            std::string tagColor;
        };

        std::unordered_map<std::string, std::vector<snippetBaseClass*>> storage;
        std::unordered_map<std::string,tag>tagStorage;
        char tagFile[500]="tagDat.cdh";
        bool noTags;

    public:
        tagHolder(int n){
            if(n==0){
                noTags=true;
                return;
            }
            assist::make_appData_filePath(tagFile);
            std::ifstream inFile(tagFile, std::ios::in);
            if (!inFile.is_open()){
                std::cerr<<"failed to open the tagDat.cdh file\n";
                return;
            }
            for (int var = 0; var < n; ++var) {
                std::string str;
                std::getline(inFile, str);
                qDebug("got line: %s",str.c_str());
                //input logic is incomplete still
            }
        }

        void insert(snippetBaseClass* snippet){
            std::vector<std::string> gotTags=snippet->getTags();
            for (int i = 0; i < gotTags.size(); i++) {
                if(tagStorage[gotTags[i]].hasData)continue;
                storage[gotTags[i]].push_back(snippet);
            }
        }

        std::vector<snippetBaseClass*>& getSnippetsFromTag(std::string tag){
            return storage[tag];
        }
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QFont getFont(std::string str);
    void setMainIndex(int n);
    void readData();
    static int firstTimeInit();

private slots:
    void on_sidebarButton_clicked();

    void on_AddnewSidebarButton_clicked();

    void on_SearchSidebarButton_clicked();

    void on_EditorsSidebarButton_clicked();

    void on_BrowseSidebarButton_clicked();

    void on_usernameAndMainSettingsButton_clicked();

    void on_centralSearchIcon_clicked();

    void on_centralSearchBoxLE_returnPressed();

private:
    Ui::MainWindow *ui;
    void loadConfig();
    char coreUserConfig[1000];

protected:
    QFont CutiveMonoFont;
    QFont CreteRoundFont;

    void loadCustomFonts();
    void centreSidebarButtons();
    void setSidebarButtonIcons();
    void prepareCentralArea();

};
#endif // MAINWINDOW_H
