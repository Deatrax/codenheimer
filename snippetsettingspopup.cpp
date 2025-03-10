#include "snippetsettingspopup.h"
#include "qgraphicseffect.h"
#include "qpropertyanimation.h"
#include "ui_snippetsettingspopup.h"

snippetSettingsPopup::snippetSettingsPopup(MainWindow *mainwindow, QWidget *parent)
    : QWidget(parent)
    ,ui(new Ui::snippetSettingsPopup)
    , masterWindow(mainwindow)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);

    CreteRound=masterWindow->getFont("Crete");
    CreteRound.setPointSize(13);
    for (QLabel* label : this->findChildren<QLabel*>()) {
        label->setFont(CreteRound);
    }
    ui->commitChangesButton->setFont(CreteRound);
    ui->deleteSnippetButton->setFont(CreteRound);
    CreteRound.setPointSize(11);
    ui->newNameBox->setFont(CreteRound);
    ui->tagSelectorComboBox->setFont(CreteRound);
    ui->isLockedCheckbox->setIcon(QIcon(":/images/lockIcon.svg"));
    ui->isLockedCheckbox->setIconSize(QSize(18,18));
    CreteRound.setPointSize(16);
    ui->titleLabel->setFont(CreteRound);
    setWindowTitle("Snippet Settings Dialogue");

    isEdited=false;
    initialLock=false;
    tagsChanged=false;
    fl=new FlowLayout(ui->tagsScrollArea,5,4,4);

}

snippetSettingsPopup::~snippetSettingsPopup()
{
    qDebug("Snippet settings diag closed");
    delete ui;
}

void snippetSettingsPopup::assign(snippetBaseClass* snippet){
    assignedSnippet=snippet;
    if (!assignedSnippet) {
        qDebug() << "No assigned snippet! Avoiding crash.";
        return;
    }
    std::vector <std::string>* tags=assignedSnippet->getInfo(name,lang);
    // ui->titleLabel

    ui->titleLabel->setText(QString(name.c_str())+"'s settings");
    initialLock=assignedSnippet->isLocked();
    if(initialLock)
        ui->isLockedCheckbox->setCheckState(Qt::Checked);
    else
        ui->isLockedCheckbox->setCheckState(Qt::Unchecked);

    std::vector<string> langs=masterWindow->getLangList();
    for(auto& it:langs){
        ui->langComboBox->addItem(QString(it.c_str()));
    }
    ui->langComboBox->setCurrentText(QString(lang.c_str()));
    ui->langComboBox->setStyleSheet(
        "QComboBox {"
        "   qproperty-alignment: AlignCenter;"  // Centers text horizontally and vertically
        "   border:3px solid black;"
        "}"
    );


    std::vector<string> tagList=masterWindow->getTagList();
    for(auto& it:tagList){
        ui->tagSelectorComboBox->addItem(QString(it.c_str()));
    }
    ui->tagSelectorComboBox->addItem(QString("Select"));
    ui->tagSelectorComboBox->setCurrentText(QString("Select"));
    ui->tagSelectorComboBox->setStyleSheet(
        "QComboBox {"
        "   qproperty-alignment: AlignCenter;"  // Centers text horizontally and vertically
        "   border:3px solid black;"
        "}"
    );

    presentTags.clear();

    for(auto& str: *tags){
        std::string name="dickpbfdb";
        std::string color="red";
        masterWindow->getTagInfo(str, name, color);
        if(name=="NoTag") continue;
        tagViewer* tv=new tagViewer(this);
        tv->setTag(name, color);
        fl->addWidget(tv);
        presentTags.insert(str);
    }
    initialTags=presentTags;
    ui->tagsScrollArea->setLayout(fl);

    isEdited=false;
    ui->tagAddButton->hide();
    ui->tagRemoveButton->hide();
}


void snippetSettingsPopup::on_newNameBox_textChanged(const QString &arg1)
{
    isEdited=true;
}


void snippetSettingsPopup::on_isLockedCheckbox_stateChanged(int arg1)
{
    isEdited=true;
}

bool snippetSettingsPopup::isTagChanged() {
    bool tagsAreSame = true;

    auto it1 = presentTags.begin();
    auto it2 = initialTags.begin();

    while (it1 != presentTags.end() && it2 != initialTags.end()) {
        if (*it1 < *it2) {
            tagsAreSame = false;
            break;  // Early exit
        } else if (*it1 > *it2) {
            tagsAreSame = false;
            break;  // Early exit
        } else {
            ++it1;
            ++it2;
        }
    }

    // If extra elements remain in either set, tags have changed
    if (it1 != presentTags.end() || it2 != initialTags.end()) {
        tagsAreSame = false;
    }

    return !tagsAreSame;  // Return true if tags are different
}


void snippetSettingsPopup::on_commitChangesButton_clicked()
{
    QString str= ui->newNameBox->text();
    if(masterWindow->containsSpaces(str)) {
        masterWindow->warnUser("Please use name without spaces");
        return;
    }


    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Exit",
                                  "Are you sure you want to save?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {

        bool nameCH=false;
        bool langCH=false;
        std::string filename;

        if(name!=ui->newNameBox->text().toStdString() && ui->newNameBox->text().toStdString()!=""){
            filename = masterWindow->generateUniqueFilename(ui->newNameBox->text(), ui->langComboBox->currentText() , 2 , assignedSnippet->getOldFilename() , assignedSnippet );
            nameCH=true;
            masterWindow->renameSnippet(ui->newNameBox->text().toStdString() , assignedSnippet);
            name = ui->newNameBox->text().toStdString();
        }
        else{
            filename=assignedSnippet->getOldFilename();
        }


        if(lang != ui->langComboBox->currentText().toStdString()){
            lang=ui->langComboBox->currentText().toStdString();
            masterWindow->snipetLangChanged( assignedSnippet, lang);
        }

        std::vector<std::string> newTags;
        for (auto &it : presentTags) {
            newTags.push_back(it);
        }

        initialLock=ui->isLockedCheckbox->checkState();

        // this->setWindowModality(Qt::NonModal);
        this->hide();
        assignedSnippet->updateSnippetDetails(name,filename,newTags,lang,initialLock);
        this->show();
        // this->setWindowModality(Qt::ApplicationModal);

        // if(nameCH)
        if(isTagChanged()) masterWindow->tagChanged(assignedSnippet);

        ui->titleLabel->setText(QString(name.c_str())+"'s settings");
        isEdited=false;
    } else {

        //do nothing
    }
}

void snippetSettingsPopup::animate(){

    QString str=ui->titleLabel->text();
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(ui->titleLabel);
    ui->titleLabel->setGraphicsEffect(effect);

    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(1000); // Duration in milliseconds
    animation->setLoopCount(2);   // Number of flashes (2 flashes = 4 opacity changes)
    animation->setKeyValueAt(0, 1.0);   // Fully visible
    animation->setKeyValueAt(0.5, 0.0); // Fully invisible
    animation->setKeyValueAt(1, 1.0);   // Fully visible again

    // Change the background color to red during the animation
    QTimer::singleShot(100, [this]() {
        ui->titleLabel->setStyleSheet(
            "background-color: red;"  // Change to red
            "color: black;"
            "border-radius:10px;"
            );
        ui->titleLabel->setText("Uncommitted Changes!");
    });

    // Reset the background color back to normal after the animation finishes
    connect(animation, &QPropertyAnimation::finished, this, [this, str]() {
        ui->titleLabel->setStyleSheet(
            "color:white;"
            "background:#303755;"
            "border-radius:10px;"
            );
        ui->titleLabel->setText(str);
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}


void snippetSettingsPopup::closeEvent(QCloseEvent *event) {
    if(isEdited){
        animate();
        event->ignore();
    }
    else{
        this->deleteLater();
        event->accept();
    }
}

void snippetSettingsPopup::on_discardChangesButton_clicked()
{
    isEdited = false;

    // Clear tagViewer widgets safely
    while (QLayoutItem* item = ui->tagsScrollArea->layout()->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
    // ui->tagSelectorComboBox->layout()->deleteLater();

    presentTags.clear();  // Reset present tags list

    // Clear combo boxes
    ui->langComboBox->clear();
    ui->langComboBox->setCurrentIndex(-1);
    ui->tagSelectorComboBox->clear();
    ui->tagSelectorComboBox->setCurrentIndex(-1);

    // Ensure assignedSnippet is valid before calling assign()
    if (!assignedSnippet) {
        qDebug() << "No assigned snippet! Avoiding crash.";
        return;
    }

    // Reassign snippet data
    assign(assignedSnippet);
}



void snippetSettingsPopup::on_langComboBox_currentIndexChanged(int index)
{
    isEdited=true;
}


void snippetSettingsPopup::on_tagAddButton_clicked()
{
    std::string selectedTag = ui->tagSelectorComboBox->currentText().toStdString();
    if(std::find(presentTags.begin(),presentTags.end(),selectedTag) != presentTags.end()){
        qDebug()<<"this tag is already added";
        return;
    }
    presentTags.insert(selectedTag);

    std::string name="dickpbfdb";
    std::string color="red";
    masterWindow->getTagInfo(selectedTag, name, color);
    tagViewer* tv=new tagViewer(this);
    tv->setTag(name, color);
    ui->tagsScrollArea->layout()->addWidget(tv);
    isEdited=true;
    qDebug()<<"added tag: "<<ui->tagSelectorComboBox->currentText().toStdString();
    on_tagSelectorComboBox_currentTextChanged(QString(selectedTag.c_str()));
}


void snippetSettingsPopup::on_tagRemoveButton_clicked()
{
    std::string selectedTag = ui->tagSelectorComboBox->currentText().toStdString();
    if(std::find(presentTags.begin(),presentTags.end(),selectedTag) == presentTags.end()){
        qDebug()<<"this tag is already removed";
        return;
    }
    presentTags.erase(selectedTag);

    for (int i = 0; i < ui->tagsScrollArea->layout()->count(); ++i) {
        QLayoutItem* item = ui->tagsScrollArea->layout()->itemAt(i);
        if (!item) continue;

        tagViewer* widget = qobject_cast<tagViewer*>(item->widget());
        if (widget) {
            // Do something with the widget
            if(widget->getTagName().toStdString()==selectedTag){
                widget->deleteLater();
                break;
            }
        }
    }


    isEdited=true;
    qDebug()<<"removed tag: "<<ui->tagSelectorComboBox->currentText().toStdString();
    on_tagSelectorComboBox_currentTextChanged(QString(selectedTag.c_str()));
}



void snippetSettingsPopup::on_tagSelectorComboBox_currentTextChanged(const QString &arg1)
{
    qDebug("combo box text changed");

    std::string selectedTag = arg1.toStdString();
    if(selectedTag=="Select"){
        ui->tagRemoveButton->hide();
        ui->tagAddButton->hide();
        return;
    }

    bool exists = std::find(presentTags.begin(), presentTags.end(), selectedTag) != presentTags.end();
    if (exists) {
        ui->tagRemoveButton->show();

        ui->tagAddButton->hide();
    } else {
        ui->tagAddButton->show();

        ui->tagRemoveButton->hide();
    }
}


void snippetSettingsPopup::on_deleteSnippetButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Exit",
                                  "Are you sure you want to delete the snippet? \n It cannot be undone!!",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        this->hide();
        if(masterWindow->deleteSnippet(assignedSnippet)){
            isEdited=false;
            this->close();
        }
        else{
            this->show();
            return;
        }
    }
    else return;
}

