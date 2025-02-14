#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <predefines.h>
#include <QFontDatabase>
#include "helperFunctions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <QClipboard>
#include "snippetbaseclass.h"
#include "snippetc.h"
#include "snippetcpp.h"
#include "snippetcss.h"
#include "snippetjava.h"
#include "snippetpy.h"
#include "snippetcustom.h"

#include "langholder.h"
#include "tagholder.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QFont getFont(std::string str);
    void setMainIndex(int n);
    void readData();
    /**
     * @brief makes and prepares all the folders and files in the
     *  appdata directory for the app to use when the app is first 
     *  run by the user so that the app doesn't crash when it tries
     *  to access the data files it needs.  
     * 
     * @return int - different values based on the execution of the function
     */
    static int firstTimeInit();
    void readUconfig();
    void getTagInfo(string tagName, std::string &passedName, std::string &passedColor);
    void copyToClipboard(const QString &text);
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
    /// @brief this function encapsulates all the opertations that take place automatically first when the app is started
    void loadConfig();
    char coreUserConfig[1000];

    void sandBox();
protected:
    QFont CutiveMonoFont;
    QFont CreteRoundFont;
    int tagCount; //the number of tags that exists
    int additionalTypeCount; //number of user added types or languages
    std::string vaultLocation; //location of the snippet files
    std::string username; //username of the user
    std::string hashResult; //hashresult of the password
    tagHolder* mainTagHolder; //pointer to hold a tagHolder object to be added in readData() function
    langHolder* mainLangHolder; //pointer to hold a langHolder object to be added in readData() function
    std::vector<snippetBaseClass*> mainStorage; // a vector to hold all the snippet's pointers
    QClipboard *clipboard;


    /// @brief loads all the custom fonts into the QFont objects that have been decleared in the header
    void loadCustomFonts();
    /// @brief centering the sidebar buttons
    void centreSidebarButtons();
    /// @brief assigning the icons of the sidebar buttons
    void setSidebarButtonIcons();
    /// @brief prepares the central area with the search, add new and browse islands
    void prepareCentralArea();
    /// @brief generates a snippet object like a predefined cpp, c, java, py, css or custom. 
    /// @param lang 
    /// @return returns a snippetBaseClass* that uses polymorphism to store a snippetliveclass derived object. 
    snippetBaseClass *generateSnippetObject(std::string lang);
};
#endif // MAINWINDOW_H
