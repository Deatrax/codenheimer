#ifndef TAGVIEWER_H
#define TAGVIEWER_H

#include <QWidget>

namespace Ui {
class tagViewer;
}

class tagViewer : public QWidget
{
    Q_OBJECT

public:
    explicit tagViewer(QWidget *parent = nullptr);
    ~tagViewer();

    void setTag(std::string Name, std::string Color);
    QString getTagName();
private:
    Ui::tagViewer *ui;
    QString tagName;
    QString tagColor;

};

#endif // TAGVIEWER_H
