#include "tagadder.h"
#include "ui_tagadder.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <vector>
#include <string>
#include <QComboBox>
#include <tagholder.h>


tagAdder::tagAdder(MainWindow *mainwindow, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::tagAdder)
    , masterWindow(mainwindow)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);

    previewLayout = new FlowLayout(ui->tagPVcentreBox,2);
    previewLayout->setAlignment(Qt::AlignCenter);
    ui->tagPreviewBox->setLayout(previewLayout);

    prepareAddNewComboBox();
}

tagAdder::~tagAdder()
{
    delete ui;
}



void tagAdder::prepareTagAdder()
{
    //CreteRoundFont.setPointSize(12);
}




void tagAdder::prepareAddNewComboBox()
{
    std::vector<std::string> colors = {
        "Red", "OrangeRed", "Crimson", "MediumVioletRed", "DarkRed",
        "DeepPink", "HotPink", "PaleVioletRed", "Purple", "Indigo",
        "DarkSlateBlue", "Blue", "DarkBlue", "SteelBlue", "CadetBlue",
        "DarkTurquoise", "Teal", "DarkCyan", "DarkSlateGray", "OliveDrab",
        "YellowGreen", "ForestGreen", "DarkGreen", "LimeGreen", "Lime",
        "Chartreuse", "Gold", "GoldenRod", "DarkGoldenRod", "SaddleBrown"
    };

    ui->tagColorComboBox->clear();
    ui->tagColorComboBox->addItem("Select");
    ui->tagColorComboBox->setCurrentText("Select");

    for (const auto &color : colors) {
        QString colorName = QString::fromStdString(color);
        ui->tagColorComboBox->addItem(colorName);
        ui->tagColorComboBox->setItemData(ui->tagColorComboBox->count() - 1,
                                          QBrush(QColor(colorName)), Qt::BackgroundRole);
    }

    ui->tagColorComboBox->setStyleSheet(
        "QComboBox {"
        "    qproperty-alignment: AlignCenter;"
        "    border:3px solid black;"
        "}"
        );

}



void tagAdder::on_tagColorEdit_textChanged(const QString &text)
{
    if (text.contains(",")) {
        QMessageBox::warning(this, "Invalid Input", "Hex color should not contain commas.");
        ui->tagColorEdit->clear();
        return;
    }

    if (!text.startsWith("#") && !text.isEmpty()) {
        QMessageBox::warning(this, "Invalid Hex", "Hex code must start with #.");
        ui->tagColorEdit->clear();
        return;
    }

    // ui->tagPreviewBox->setStyleSheet("background-color: " + text + "; color: white; "
    //                                                                "font-family:'Crete Round'; font-size:14px; border-radius:5px;");

    ui->tagColorComboBox->setCurrentIndex(0);
    tagColorSource = 2; // Custom color input

    updatePreview();
}




void tagAdder::on_tagTextEdit_textChanged(const QString &text)
{
    updatePreview();
}



void tagAdder::on_tagColorComboBox_currentTextChanged(const QString &arg1)
{
    ui->tagColorEdit->clear();
    QString colorName = ui->tagColorComboBox->currentText();
    // ui->tagPreviewBox->setStyleSheet("background-color: " + colorName + "; color: white; "
    //                                                                     "font-family:'Crete Round'; font-size:14px; border-radius:5px;");

    if (colorName == "Select" && ui->tagColorEdit->text().isEmpty()) {
        tagColorSource = 0; // No color selected
    } else {
        tagColorSource = 1; // ComboBox color selected
    }

    updatePreview();
}




void tagAdder::updatePreview(){
    // If text is empty, remove tag preview
    QString text=ui->tagTextEdit->text();
    if (text.isEmpty()) {
        if (previewTag) {
            previewLayout->removeWidget(previewTag);
            delete previewTag;
            previewTag = nullptr;
        }
        return;
    }

    // Get selected color from comboBox or lineEdit
    QString color = (tagColorSource == 1) ? ui->tagColorComboBox->currentText()
                                          : ui->tagColorEdit->text();

    // If a preview already exists, update its text and color
    if (previewTag) {
        previewTag->setTag(text.toStdString(), color.toStdString()); // Use setTag()
        return;
    }

    // Otherwise, create a new tagViewer object
    previewTag = new tagViewer();
    previewTag->setTag(text.toStdString(), color.toStdString()); // Use setTag()

    // Add tag to preview layout
    previewLayout->addWidget(previewTag);
}



void tagAdder::on_confirmButton_clicked()
{
    if (tagColorSource == 0) {
        QMessageBox::warning(this, "No Color Selected", "Please select a color from the dropdown or enter a custom hex code.");
        return;
    }

    QString finalColor;
    if (tagColorSource == 1) {
        finalColor = ui->tagColorComboBox->currentText();
    } else if (tagColorSource == 2) {
        finalColor = ui->tagColorEdit->text();
    }

    qDebug() << "Final selected color: " << finalColor;
    qDebug() << "Source of selection: " << tagColorSource;
     //
    if (!previewTag) {
        QMessageBox::warning(this, "Invalid Tag", "Tag preview is missing.");
        return;
    }

    // ✅ Create a copy of the preview tag
    //tagViewer *newTag = new tagViewer();
    //newTag->setTag(previewTag->getTagName().toStdString(),previewTag->getTagColor().toStdString());
    if (masterWindow) {
        //tagHolder *tag=
        masterWindow->getMainTagHolder(previewTag->getTagName().toStdString(), previewTag->getTagColor().toStdString());

        masterWindow->addTagtoList();  // updates the list widget
    }



    QString tagText= previewTag->getTagName();
    QString tagColor=previewTag->getTagColor();
    QString tagData= tagText+ "," +tagColor + ",";
    tagToVault(tagData);
    this->close(); //moved it to the end
}

void tagAdder::tagToVault(QString tagData)
{
    char vaultFilePath[assist::PATH_SIZE];
    std::strncpy(vaultFilePath, "tagDat.cdh", sizeof(vaultFilePath) - 1);
    vaultFilePath[sizeof(vaultFilePath) - 1] = '\0';
    assist::make_appData_filePath(vaultFilePath);
    if(assist::addLine(vaultFilePath,-1,tagData.toStdString()))
        masterWindow->showAutoCloseMessageBox(this,"Success!","Tag added to vault success!");
    else{
        masterWindow->warnUser("Tag failed to add in vault! \n Please check logs and contact devs");
        return;
    }
}


void tagAdder::on_cancelButton_clicked()
{
    this->close();
}

