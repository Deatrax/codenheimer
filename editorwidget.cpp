#include "editorwidget.h"
#include "ui_editorwidget.h"
#include <qscrollbar.h>

editorWidget::editorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::editorWidget)
{
    ui->setupUi(this);

    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->lineNo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding); // Prevents expansion
    ui->lineNo->setAlignment(Qt::AlignTop | Qt::AlignLeft);  // Aligns text properly

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
