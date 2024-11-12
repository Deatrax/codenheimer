#ifndef SNIPPETPREVIEWBOX_H
#define SNIPPETPREVIEWBOX_H

#include <QWidget>
#include <mainwindow.h>
namespace Ui {
class snippetPreviewBox;
}

class snippetPreviewBox : public QWidget
{
    Q_OBJECT

public:
    explicit snippetPreviewBox(MainWindow *mainwindow, QWidget *parent = nullptr);
    ~snippetPreviewBox();

private:
    Ui::snippetPreviewBox *ui;
    MainWindow *masterWindow;
    QFont CutiveMonoFont;
};

#endif // SNIPPETPREVIEWBOX_H
