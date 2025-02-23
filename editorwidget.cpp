#include "editorwidget.h"
#include "qlistwidget.h"
#include "snippetpreviewbox.h"
#include "snippetsettingspopup.h"
#include "ui_editorwidget.h"
#include <qscrollbar.h>

editorWidget::editorWidget(MainWindow* mainwindow, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::editorWidget)
    , mainWindow(mainwindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->lineNo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding); // Prevents expansion
    ui->lineNo->setAlignment(Qt::AlignTop | Qt::AlignLeft);  // Aligns text properly

    ui->textEdit->setWordWrapMode(QTextOption::NoWrap); // Disable word wrapping
    ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded); // Show scrollbar when needed

    //text highlighting=====================================
    // QTextEdit *textEdit = new QTextEdit;
    // KeywordHighlighter *highlighter = new KeywordHighlighter(textEdit->document());
    // textEdit->setPlainText("int main() { for (int i = 0; i < 10; ++i) {} }");
    // textEdit->show();


    //KeywordHighlighter *highlighter = new KeywordHighlighter(ui->textEdit->document());
    ui->textEdit->setPlainText("int main() { for (int i = 0; i < 10; ++i) {} }");
    ui->lineNo->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    connect(ui->textEdit->verticalScrollBar(), &QScrollBar::valueChanged, this, &editorWidget::synchronizeScroll);
    ui->lineNo->setContentsMargins(0, 5, 0, 0);  // Top margin of 5 pixels
    on_textEdit_textChanged();

    ui->editorCopyButton->setIcon(QIcon(":images/copyButton.svg"));
    ui->editorCopyButton->setIconSize(QSize(18,18));

    ui->editorSnippetSettingsButton->setIcon(QIcon(":/images/settingsIcon.svg"));
    ui->editorSnippetSettingsButton->setIconSize(QSize(21, 21));

    ui->editorSaveButton->setIcon(QIcon(":/images/saveIcon.svg"));
    ui->editorSaveButton->setIconSize(QSize(19, 19));

    ui->tabCloseButton->setIcon(QIcon(":/images/closeIcon.svg"));
    ui->tabCloseButton->setIconSize(QSize(20,20));
}

editorWidget::~editorWidget()
{
    delete ui;
}


void editorWidget::on_textEdit_textChanged()
{
    QStringList lineNumbers;
    int lineCount = ui->textEdit->document()->blockCount();

    for (int i = 1; i <= lineCount; ++i) {
        lineNumbers << QString::number(i);
    }

    ui->lineNo->setText(lineNumbers.join("\n"));

    //ui->lineNo->setFixedHeight(ui->textEdit->document()->size().height());
}


void editorWidget::synchronizeScroll(int value) {
    // Scroll the label's content to match the textEdit
    ui->lineNo->move(ui->lineNo->x(), -value);
}

QTextDocument* editorWidget::syntaxHighlightTarget(){
    return ui->textEdit->document();
}

void editorWidget::assign(snippetBaseClass* snipObj, bool isOld){
    thisSnippet=snipObj;
    thisSnippet->setEditor(ui->textEdit->document());
    if(isOld) ui->textEdit->setText(QString(thisSnippet->getSnippet().c_str()));
    else ui->textEdit->setText("");
}

void editorWidget::tellIdx(int i){
    idx=i;
}

void editorWidget::on_editorSnippetSettingsButton_clicked()
{
    mainWindow->warnUser("Jessan was supposed to make this!! but now i did");
    snippetSettingsPopup* pop=new snippetSettingsPopup(mainWindow);
    pop->assign(thisSnippet);
    pop->show();
}


void editorWidget::on_editorCopyButton_clicked()
{
    QString str=ui->textEdit->toPlainText();
    thisSnippet->saveSnippetToFile(str.toStdString());
    mainWindow->copyToClipboard(str);
}


void editorWidget::on_editorSaveButton_clicked()
{
    QString str=ui->textEdit->toPlainText();
    if(thisSnippet->saveSnippetToFile(str.toStdString())){
        mainWindow->showAutoCloseMessageBox(this, "Success!","The snippet was saved successfully!");
    }
}


void editorWidget::on_tabCloseButton_clicked()
{
    mainWindow->closeTab();

}
