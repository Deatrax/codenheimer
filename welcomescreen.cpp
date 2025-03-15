#include "welcomescreen.h"
#include "ui_welcomescreen.h"



welcomeScreen::welcomeScreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::welcomeScreen)
{
    ui->setupUi(this);

    int fontId = QFontDatabase::addApplicationFont(":/fonts/CutiveMono-Regular.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        CutiveMonoFont=QFont(family);
        CutiveMonoFont.setPointSize(20);
    } else {
        qDebug() << "Font loading failed!";
    }


    int fontId2 = QFontDatabase::addApplicationFont(":/fonts/CreteRound-Regular.ttf");
    if (fontId2 != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId2).at(0);
        CreteRoundFont=QFont(family);
        CreteRoundFont.setPointSize(36);
    } else {
        qDebug() << "Font loading failed!";
    }


    ui->welcomeTitle->setFont(CreteRoundFont);
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(ui->welcomeTitle);
    ui->welcomeTitle->setGraphicsEffect(effect);
    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(1000);
    animation->setLoopCount(1);
    animation->setKeyValueAt(0, 0);
    animation->setKeyValueAt(1, 1.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    ui->startButton->setIcon(QIcon(":/images/rightCircularArrow.svg"));
    ui->startButton->setIconSize(QSize(36, 36));
    QGraphicsOpacityEffect* effect2 = new QGraphicsOpacityEffect(ui->startButton);
    ui->startButton->setGraphicsEffect(effect2);
    QPropertyAnimation* animation2 = new QPropertyAnimation(effect2, "opacity");
    animation2->setDuration(2000);
    animation2->setLoopCount(1);
    animation2->setKeyValueAt(0, 0);
    animation2->setKeyValueAt(0.5, 0);
    animation2->setKeyValueAt(1, 1.0);
    animation2->start(QAbstractAnimation::DeleteWhenStopped);

    ui->page1innerStack->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
    applyFontToChildren(ui->page1InnerStack_page2, CutiveMonoFont);

    applyNavigationButtonIcons();
    for (int i = 2; i <= 9; ++i) {
        QLabel* label = nullptr;

        if (i == 2) {
            label = findChild<QLabel*>("label");  // Page 2 has "label" instead of "label_1"
        } else {
            label = findChild<QLabel*>(QString("label_%1").arg(i - 1));
        }

        if (label) {
            QString imagePath = QString(":/images/walkthrough/walkthrough%1.png").arg(i - 1);
            QPixmap pixmap(imagePath);
            int scaledWidth = 2184 / 3;  // Reduce width by a factor of 2
            int scaledHeight = 1500 / 3; // Reduce height by a factor of 2
            label->setPixmap(pixmap.scaled(scaledWidth, scaledHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            label->setScaledContents(true);
        }
    }
}

welcomeScreen::~welcomeScreen()
{
    delete ui;
}


void welcomeScreen::applyNavigationButtonIcons() {
    QIcon rightArrow(":/images/rightCircularArrow.svg");
    QIcon leftArrow(":/images/leftCircularArrow.svg");
    QSize iconSize(36, 36);

    QList<QPushButton*> nextButtons = {
        ui->page2nxt, ui->page3nxt, ui->page4nxt, ui->page5nxt,
        ui->page6nxt, ui->page7nxt, ui->page8nxt, ui->page9nxt
    };

    QList<QPushButton*> prevButtons = {
        ui->page3prev, ui->page4prev, ui->page5prev, ui->page6prev,
        ui->page7prev, ui->page8prev, ui->page9prev
    };

    for (auto button : nextButtons) {
        button->setIcon(rightArrow);
        button->setIconSize(iconSize);
    }

    for (auto button : prevButtons) {
        button->setIcon(leftArrow);
        button->setIconSize(iconSize);
    }
}


void welcomeScreen::on_startButton_clicked()
{
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(ui->welcomeTitle);
    ui->welcomeTitle->setGraphicsEffect(effect);
    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(1000);
    animation->setLoopCount(1);
    animation->setKeyValueAt(0, 1.0);
    animation->setKeyValueAt(1, 0);

    QGraphicsOpacityEffect* effect2 = new QGraphicsOpacityEffect(ui->startButton);
    ui->startButton->setGraphicsEffect(effect2);
    QPropertyAnimation* animation2 = new QPropertyAnimation(effect2, "opacity");
    animation2->setDuration(1000);
    animation2->setLoopCount(1);
    animation2->setKeyValueAt(0, 1.0);
    animation2->setKeyValueAt(0.5, 1.0);
    animation2->setKeyValueAt(1, 0);


    QGraphicsOpacityEffect* effect3 = new QGraphicsOpacityEffect(ui->page1InnerStack_page2container);
    ui->page1InnerStack_page2container->setGraphicsEffect(effect3);
    QPropertyAnimation* animation3 = new QPropertyAnimation(effect3, "opacity");
    animation3->setDuration(3000);
    animation3->setLoopCount(1);
    animation3->setKeyValueAt(0, 0);
    animation3->setKeyValueAt(0.66, 0);
    animation3->setKeyValueAt(1, 1);

    // Capture `this` and execute the lambda when animation2 finishes
    connect(animation2, &QPropertyAnimation::finished, this, [this]() {
        qDebug() << "All animations finished!";
        // Perform any additional actions here, like hiding UI elements
        ui->welcomeTitle->hide();
        ui->startButton->hide();
        ui->page1innerStack->setCurrentIndex(1);
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped);
    animation2->start(QAbstractAnimation::DeleteWhenStopped);
    animation3->start(QAbstractAnimation::DeleteWhenStopped);
}



void welcomeScreen::applyFontToChildren(QWidget* parent, const QFont& font) {
    if (!parent) return;

    parent->setFont(font);  // Apply font to the parent itself

    for (QObject* child : parent->children()) {
        QWidget* childWidget = qobject_cast<QWidget*>(child);
        if (childWidget) {
            childWidget->setFont(font);  // Apply font to child widget
            applyFontToChildren(childWidget, font);  // Recursively apply to grandchildren
        }
    }
}

void welcomeScreen::on_showPasswordButton_pressed()
{
    ui->newPasswordEdit->setEchoMode(QLineEdit::Normal);
}


void welcomeScreen::on_showPasswordButton_released()
{
    ui->newPasswordEdit->setEchoMode(QLineEdit::Password);
}


void welcomeScreen::on_userUpdateButton_clicked()
{
    // QString usr= ui->usernameEdit->text();
    // QString hashres=QString(QCryptographicHash::hash(ui->newPasswordEdit->text().toUtf8(), QCryptographicHash::Sha256).toHex()) ;

    // QString company="AronoxStudios";
    // QString appName="Codenheimer";
    // QSettings settings(company, appName);
    // settings.setValue("username", usr);
    // settings.setValue("hashres", hashres);
    // settings.setValue("vault", "default");
    // settings.setValue("tag",  0);
    // settings.setValue("type", 0);
    // settings.setValue("showPerPage", 10);
    // settings.setValue("loginRun",false);
    // settings.setValue("trayIcon",false);


    ui->stackedWidget->setCurrentIndex(1);
}


void welcomeScreen::on_page2nxt_clicked() {
    ui->stackedWidget->setCurrentIndex(2);
}

void welcomeScreen::on_page3nxt_clicked() {
    ui->stackedWidget->setCurrentIndex(3);
}

void welcomeScreen::on_page3prev_clicked() {
    ui->stackedWidget->setCurrentIndex(1);
}

void welcomeScreen::on_page4nxt_clicked() {
    ui->stackedWidget->setCurrentIndex(4);
}

void welcomeScreen::on_page4prev_clicked() {
    ui->stackedWidget->setCurrentIndex(2);
}

void welcomeScreen::on_page5nxt_clicked() {
    ui->stackedWidget->setCurrentIndex(5);
}

void welcomeScreen::on_page5prev_clicked() {
    ui->stackedWidget->setCurrentIndex(3);
}

void welcomeScreen::on_page6nxt_clicked() {
    ui->stackedWidget->setCurrentIndex(6);
}

void welcomeScreen::on_page6prev_clicked() {
    ui->stackedWidget->setCurrentIndex(4);
}

void welcomeScreen::on_page7nxt_clicked() {
    ui->stackedWidget->setCurrentIndex(7);
}

void welcomeScreen::on_page7prev_clicked() {
    ui->stackedWidget->setCurrentIndex(5);
}

void welcomeScreen::on_page8nxt_clicked() {
    ui->stackedWidget->setCurrentIndex(8);
}

void welcomeScreen::on_page8prev_clicked() {
    ui->stackedWidget->setCurrentIndex(6);
}

void welcomeScreen::on_page9nxt_clicked() {
    ui->stackedWidget->setCurrentIndex(9);
}

void welcomeScreen::on_page9prev_clicked() {
    ui->stackedWidget->setCurrentIndex(7);
}
