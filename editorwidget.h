#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QWidget>

namespace Ui {
class editorWidget;
}

class editorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit editorWidget(QWidget *parent = nullptr);
    ~editorWidget();

private slots:
    void on_textEdit_textChanged();
private:
    Ui::editorWidget *ui;
    void synchronizeScroll(int value);
};

#endif // EDITORWIDGET_H
