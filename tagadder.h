#ifndef TAGADDER_H
#define TAGADDER_H

#include <QWidget>
#include <QFontDatabase>
#include <QFont>
#include "tagviewer.h"
#include <QVBoxLayout>
#include "flowlayout.h"
#include "mainwindow.h"
#include "tagholder.h"

namespace Ui {
class tagAdder;
}

class tagAdder : public QWidget
{
    Q_OBJECT

public:
    explicit tagAdder(MainWindow *mainwindow, QWidget *parent = nullptr);
    ~tagAdder();

    void prepareTagAdder();
    void prepareAddNewComboBox();

private slots:

    void on_tagColorEdit_textChanged(const QString &arg1);

    void on_confirmButton_clicked();

    void on_tagTextEdit_textChanged(const QString &text);

    void on_tagColorComboBox_currentTextChanged(const QString &arg1);

    void on_cancelButton_clicked();

private:
    Ui::tagAdder *ui;
    int tagColorSource = 0;  // 0 = No color, 1 = ComboBox, 2 = Custom input
    FlowLayout *previewLayout = nullptr;  // Layout for tag preview
    tagViewer *previewTag = nullptr;  // Tag preview widget
    MainWindow* masterWindow;
    void updatePreview();
    void tagToVault(QString tagData);
};

#endif // TAGADDER_H
