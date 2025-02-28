#ifndef CRYPTOGRAPHICAGENT_H
#define CRYPTOGRAPHICAGENT_H

#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

namespace Ui {
class cryptographicAgent;
}

class cryptographicAgent : public QWidget
{
    Q_OBJECT

public:
    explicit cryptographicAgent(QWidget *parent = nullptr);
    ~cryptographicAgent();

    int encryptToFile(QString fileName, QString data);
    QString decryptFromFile(QString fileName);


    std::string setPassFirstTime();
    void setHash(std::string str);
    std::string getHash();
private slots:
    void on_cofirmButton_clicked();



private:

    Ui::cryptographicAgent *ui;
    QString decryptFileToString(const QString &filePath, const QByteArray &key, const QByteArray &iv);

    void encryptStringToFile(const QString &data, const QString &filePath, const QByteArray &key, const QByteArray &iv);

    bool deriveKeyAndIV(const QString &password, QByteArray &key, QByteArray &iv);

    void showUI(int mode);

    void showUI();

    char password[256];
    void storePassword(QString pass);
    void secureErase(char *buffer, size_t size);
    bool hasPassword;
    std::string hashResult;
    int actionContinue;
    bool firstTime=false;

    QString getAppDataFilePath(const QString &filename);
    QString hashPassword(const QString &pass);
signals:
    void passwordSet();
};

#endif // CRYPTOGRAPHICAGENT_H
