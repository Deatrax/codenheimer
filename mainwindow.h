#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <predefines.h>
#include <QFontDatabase>
#include "flowlayout.h"
#include "helperFunctions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <QClipboard>
#include <QToolTip>
#include <QMessageBox>
#include <QTimer>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <unordered_map>
#include <QSystemTrayIcon>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#include "snippetbaseclass.h"
#include "snippetc.h"
#include "snippetcpp.h"
#include "snippetcss.h"
#include "snippetjava.h"
#include "snippetpy.h"
#include "snippetcustom.h"
#include "searchsyetem.h"

#include "langholder.h"
#include "tagholder.h"
//settingspage-prince
#include <QVBoxLayout>
#include <qlistwidget.h>
#include <tagviewer.h>
#include "cryptographicagent.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


// class editorWidget; //forward decleration




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
    void addNewAction();
    void showAutoCloseMessageBox(QWidget *parent, QString errTitle, QString msg);
    void warnUser(QString str);
    void closeTab();
    friend class editorWidget;

    std::vector<string> getLangList();
    std::vector<std::string> getTagList();
    std::string generateUniqueFilename(const QString &name, const QString &lang, int mode,std::string oldFilename="",snippetBaseClass *obj=std::nullptr_t());
    bool deleteSnippet(snippetBaseClass *obj);
    void renameSnippet(std::string newName , snippetBaseClass *obj);
    void snipetLangChanged(snippetBaseClass *obj, string lang);
    void tagChanged(snippetBaseClass *obj);
    bool containsSpaces(QString &str);
    void addTagtoList();//tagViewer *tag);
    void getMainTagHolder(const std::string &tagName, const std::string &tagColor);
    void test();
    void encryptText(QString file, QString data);
    QString decryptText(QString fileName);
    void scheduleDeletion(int lineNumber, const QString &filePath);
    void openSnippetInEditor(snippetBaseClass *snipObj, QString &tabname, bool isOld);
    void scheduleTagDeletion(int lineNumber);
    void applyFilter(std::string text, int type);
    void removeFilter(std::string text, int type);
    
    private slots:
    void on_sidebarButton_clicked();

    void on_AddnewSidebarButton_clicked();

    void on_SearchSidebarButton_clicked();

    void on_EditorsSidebarButton_clicked();

    void on_BrowseSidebarButton_clicked();

    void on_usernameAndMainSettingsButton_clicked();

    void on_centralSearchIcon_clicked();

    void on_centralSearchBoxLE_returnPressed();

// <<<<<<< ryexocious-making-search-page
    void on_searchBoxLineEdit_textChanged(const QString &arg1);
    
    void on_newSnippetNameBox_textChanged(const QString &arg1);

    void on_newSnippetNameBox_returnPressed();

    void on_addNewButton_clicked();

    void on_EditorsDefaultTabButton_clicked();

    void on_downarrow_clicked();

    void on_centralBrowseButton_clicked();

    void on_testCryptoButton_clicked();

    void on_testDecryptButton_clicked();

    void on_snippetSettingsTestButton_clicked();

    void on_snippetSettingsOnSearchPage_clicked();

    void on_addTagButton_clicked();



    void on_sysTrayCheckBox_clicked(bool checked);

    void on_OpenAtLoginCheckBox_clicked(bool checked);

    void on_removeTagButton_clicked();

    void on_pushButton_2_clicked();

    void on_browseShortCutButton_clicked();

    void on_browseBackButton_clicked();

    void on_perPageSee_valueChanged(int arg1);

    void on_nextPageButton_clicked();

    void on_previousPageButton_clicked();


    void on_mofobutton_clicked();

    void on_userUpdateButton_clicked();

    void on_newPasswordEdit_returnPressed();

    void on_showPasswordButton_pressed();
    void on_showPasswordButton_released();
private:
    Ui::MainWindow *ui;
    /// @brief this function encapsulates all the opertations that take place automatically first when the app is started
    void loadConfig();
    char coreUserConfig[1000];

    void sandBox();
    void searchPageSearchbar();
    searchSystem *searchObj;


// >>>>>>> main
    QFont CutiveMonoFont;
    QFont CreteRoundFont;
    int tagCount; //the number of tags that exists
    int additionalTypeCount; //number of user added types or languages
    std::string vaultLocation; //location of the snippet files
    std::string username; //username of the user
    std::string hashResult; //hashresult of the password
    tagHolder* mainTagHolder; //pointer to hold a tagHolder object to be added in readData() function
    langHolder* mainLangHolder; //pointer to hold a langHolder object to be added in readData() function
    std::map<std::string, snippetBaseClass*> lockedStorage;
    // this was deprecated because filne name storage was already doing it = std::vector<snippetBaseClass*> mainStorage; // a vector to hold all the snippet's pointers
    QClipboard *clipboard;
    std::unordered_map<std::string,snippetBaseClass*> filenameStorage;
    int lineNum;
    static QString company;
    static QString appName;
    cryptographicAgent* Julius;
    FlowLayout *browseLangFL;
    FlowLayout *browseTagFL;

    //========filters
    std::vector< std::string> langFilters;
    std::vector< std::string> tagFilters;




    void prepareAddNewComboBox();
    void saveToSettings(const QString &username, const QString &hashResult, const QString &vault, int tag, int type);
    void prepareSettingsPage();
    //QListWidget *tagListWidget;  // QListWidget to hold tags
    //QVBoxLayout *taglayout;

    //system tray related functions
    void createTrayActions();
    void createSysTray();
    void setClickableOptions(bool visible);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void closeEvent(QCloseEvent *event) override;
    void trayVisibility(bool flag);


    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QMenu *trayIconMenu;
    QSystemTrayIcon *trayIcon;
    bool trayEnabled;

    void setAutoStartWindows(bool flag);
    bool loginEnabled;

    void prepareBrowsePage();
    void applyFontToChildren(QWidget *parent, const QFont &font);
    void updateBrowseView();

    void updateBrowseView(bool flag);
    std::vector<snippetBaseClass *> getFilteredSnippets(const std::vector<std::string> &langFilters, const std::vector<std::string> &tagFilters, const std::vector<snippetBaseClass *> &snippets, langHolder* langDB, tagHolder* tagDB);



    void savePendingTagDeletes();

    void loadPendingTagDeletes();

    void completeTagDeletes();
    QSet<int> pendingTagDeletions;   // Stores line numbers of tags to delete
    QString tagTempFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/pending_tag_deletes.tmp";;         // Path to store pending deletions for tags

    protected:
    
    void completeDeletes();
    void loadPendingDeletes();
    void savePendingDeletes();


    std::map<int, QString, std::greater<int>> pendingDeletions;
    QString tempFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/pending_deletes.tmp";



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
    void addNewSnippet(QString name, QString lang);
};
#endif // MAINWINDOW_H
