#ifndef CRYPTOGRAPHICAGENT_H
#define CRYPTOGRAPHICAGENT_H

#include <QWidget>

namespace Ui {
class cryptographicAgent;
}

class cryptographicAgent : public QWidget
{
    Q_OBJECT

public:
    explicit cryptographicAgent(QWidget *parent = nullptr);
    ~cryptographicAgent();

private:
    Ui::cryptographicAgent *ui;
};

#endif // CRYPTOGRAPHICAGENT_H
