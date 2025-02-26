#include "cryptographicagent.h"
#include "ui_cryptographicagent.h"

cryptographicAgent::cryptographicAgent(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::cryptographicAgent)
{
    ui->setupUi(this);
    ui->confrimPassText->hide();
    ui->passwordConfirmField->hide();
}



cryptographicAgent::~cryptographicAgent()
{
    delete ui;
}
