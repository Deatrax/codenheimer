#ifndef SNIPPETPREVIEWBOX_H
#define SNIPPETPREVIEWBOX_H

#include <QWidget>
#include <mainwindow.h>
#include "snippetbaseclass.h"

namespace Ui {
class snippetPreviewBox;
}

class snippetPreviewBox : public QWidget
{
    Q_OBJECT

public:
    explicit snippetPreviewBox(MainWindow *mainwindow, QWidget *parent = nullptr);
    ~snippetPreviewBox();

    void assignSnippet(snippetBaseClass *snippet);
protected:
    void setTags();
private:
    Ui::snippetPreviewBox *ui;
    MainWindow *masterWindow;
    QFont CutiveMonoFont;
    snippetBaseClass* assignedSnippet;
    std::string name;
    std::string lang;
    std::vector <std::string>* tags;

    void setLock();
};

#endif // SNIPPETPREVIEWBOX_H
