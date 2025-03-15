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
    snippetBaseClass *getSnippetObj();
    QString getNam();
protected:
    void setTags();
    void addCopyButton();
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private slots:


    void on_copyButton_clicked();

private:
    Ui::snippetPreviewBox *ui;
    MainWindow *masterWindow;
    QFont CutiveMonoFont;
    snippetBaseClass* assignedSnippet;
    std::string name;
    std::string lang;
    std::vector <std::string>* tags;

    void setLock();
    bool eventFilter(QObject *watched, QEvent *event);
    void readSnippet(std::string &str);
};

#endif // SNIPPETPREVIEWBOX_H
