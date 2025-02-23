#ifndef SNIPPETSETTINGSPOPUP_H
#define SNIPPETSETTINGSPOPUP_H

#include "mainwindow.h"
#include <QWidget>
#include <QCloseEvent>
#include "tagviewer.h"
#include "flowlayout.h"

namespace Ui {
class snippetSettingsPopup;
}

class snippetSettingsPopup : public QWidget
{
    Q_OBJECT

public:
    explicit snippetSettingsPopup(MainWindow *mainwindow, QWidget *parent = nullptr);
    ~snippetSettingsPopup();

    void assign(snippetBaseClass *snippet);
private slots:
    void on_newNameBox_textChanged(const QString &arg1);

    void on_isLockedCheckbox_stateChanged(int arg1);

    void on_commitChangesButton_clicked();

    void on_discardChangesButton_clicked();

    void on_langComboBox_currentIndexChanged(int index);

    void on_tagAddButton_clicked();

    void on_tagRemoveButton_clicked();

    void on_tagSelectorComboBox_currentTextChanged(const QString &arg1);

    void on_deleteSnippetButton_clicked();

private:
    Ui::snippetSettingsPopup *ui;
    MainWindow *masterWindow;
    QFont CreteRound;
    snippetBaseClass* assignedSnippet;
    std::string name;
    std::string lang;
    std::set<std::string> presentTags;
    void animate();
    void closeEvent(QCloseEvent *event) override;
    bool isEdited;
    bool tagsChanged;
    bool initialLock;
    FlowLayout* fl;

};

#endif // SNIPPETSETTINGSPOPUP_H
