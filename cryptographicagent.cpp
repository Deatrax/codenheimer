#include "cryptographicagent.h"
#include "ui_cryptographicagent.h"

cryptographicAgent::cryptographicAgent(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::cryptographicAgent)
{
    ui->setupUi(this);
    this->hide();
}



cryptographicAgent::~cryptographicAgent()
{
    delete ui;
}

void cryptographicAgent::storePassword(QString pass) {

    strncpy(password, "MySuperSecretPassword", sizeof(password) - 1);
    qDebug() << "Password stored in memory.";

    // Schedule secureErase() to run in 5 minutes (300,000 ms)
    QTimer::singleShot(300000, [=]() { secureErase(password, sizeof(password)); });

    hasPassword=true;
}

void cryptographicAgent::secureErase(char* buffer, size_t size) {
    volatile char* p = buffer;
    while (size--) {
        *p++ = 0;
    }
    qDebug() << "Password securely erased.";
    hasPassword=false;
}


QString cryptographicAgent::decryptFileToString(const QString &filePath, const QByteArray &key, const QByteArray &iv) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Failed to open encrypted file.");
    }

    // OpenSSL decryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create OpenSSL context.");
    }

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                            reinterpret_cast<const unsigned char *>(key.data()),
                            reinterpret_cast<const unsigned char *>(iv.data()))) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize decryption.");
    }

    QByteArray encryptedData;
    QByteArray buffer(4096, 0);
    while (!file.atEnd()) {
        qint64 bytesRead = file.read(buffer.data(), buffer.size());
        if (bytesRead > 0) {
            encryptedData.append(buffer.left(bytesRead));
        } else if (bytesRead < 0) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Error reading encrypted file.");
        }
    }
    file.close();

    // Perform decryption
    QByteArray decryptedData(encryptedData.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()), 0);
    int decryptedLength = 0;
    int finalLength = 0;

    if (!EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char *>(decryptedData.data()), &decryptedLength,
                           reinterpret_cast<const unsigned char *>(encryptedData.data()), encryptedData.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error during decryption.");
    }

    if (!EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(decryptedData.data()) + decryptedLength, &finalLength)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error finalizing decryption. Wrong key/IV?");
    }

    EVP_CIPHER_CTX_free(ctx);
    decryptedData.resize(decryptedLength + finalLength);

    return QString::fromUtf8(decryptedData);
}



// Derive encryption key and IV from password
bool cryptographicAgent::deriveKeyAndIV(const QString &password, QByteArray &key, QByteArray &iv) {
    key.resize(32); // 256-bit key
    iv.resize(16);  // 128-bit IV
    QByteArray salt = "example_salt"; // You can generate a random salt for stronger security

    // Use PBKDF2 to derive the key and IV
    if (!PKCS5_PBKDF2_HMAC(password.toUtf8().data(),
                           password.length(),
                           reinterpret_cast<const unsigned char*>(salt.data()), salt.size(),
                           10000, // Iteration count
                           EVP_sha256(),
                           key.size() + iv.size(),
                           reinterpret_cast<unsigned char*>(key.data()))) {
        qWarning() << "Failed to derive key/IV.";
        return false;
    }

    iv = key.mid(32, 16); // Extract the last 16 bytes as IV
    key = key.left(32);   // First 32 bytes as the key
    return true;
}

void cryptographicAgent::encryptStringToFile(const QString &data, const QString &filePath, const QByteArray &key, const QByteArray &iv) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Failed to open file for writing.");
    }

    // OpenSSL encryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create OpenSSL context.");
    }

    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                            reinterpret_cast<const unsigned char *>(key.data()),
                            reinterpret_cast<const unsigned char *>(iv.data()))) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize encryption.");
    }

    QByteArray inputData = data.toUtf8();
    QByteArray encryptedData(inputData.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()), 0);
    int encryptedLength = 0;
    int finalLength = 0;

    // Perform encryption
    if (!EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char *>(encryptedData.data()), &encryptedLength,
                           reinterpret_cast<const unsigned char *>(inputData.data()), inputData.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error during encryption.");
    }

    if (!EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(encryptedData.data()) + encryptedLength, &finalLength)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error finalizing encryption.");
    }

    EVP_CIPHER_CTX_free(ctx);

    encryptedData.resize(encryptedLength + finalLength);

    // Write encrypted data to file
    if (file.write(encryptedData) != encryptedData.size()) {
        throw std::runtime_error("Failed to write encrypted data to file.");
    }

    file.close();
}

int cryptographicAgent::encryptToFile(QString filePath, QString data){


    if (!hasPassword) {
        QEventLoop loop;
        connect(this, &cryptographicAgent::passwordSet, &loop, &QEventLoop::quit);
        showUI(1);
        loop.exec();  // Blocks execution until passwordSet() is emitted
    }

    QByteArray key, iv;
    if (!deriveKeyAndIV(password, key, iv)) {
        QMessageBox::warning(this, "Error", "Failed to generate encryption key and IV.");
        return -1;
    }

    QString dataToEncrypt = data;
    if (dataToEncrypt.isEmpty()) {
        QMessageBox::warning(this, "Error", "No content to encrypt.");
        return -2;
    }

    try {
        encryptStringToFile(dataToEncrypt, filePath, key, iv);
        QMessageBox::information(this, "Success", "File encrypted successfully.");
    } catch (const std::runtime_error &e) {
        QMessageBox::warning(this, "Error", e.what());
        return -3;
    }
    return 0;
}

QString cryptographicAgent::decryptFromFile(QString filePath, QString data)
{
    if (!hasPassword) {
        QEventLoop loop;
        connect(this, &cryptographicAgent::passwordSet, &loop, &QEventLoop::quit);
        showUI(1);
        loop.exec();  // Blocks execution until passwordSet() is emitted
    }

    QString decryptedContent="";
    QByteArray key, iv;
    if (!deriveKeyAndIV(password, key, iv)) {
        QMessageBox::warning(this, "Error", "Failed to generate decryption key and IV.");
        return QString();
    }

    try {
        decryptedContent = decryptFileToString(filePath, key, iv);
        // ui->FileContents->setPlainText(decryptedContent); // Display decrypted content
        QMessageBox::information(this, "Success", "File decrypted successfully. CONTENTS:"+decryptedContent);
    } catch (const std::runtime_error &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
    return decryptedContent;
}

QString cryptographicAgent::hashPassword(const QString& pass){
    return QString(QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Sha256).toHex()) ;
}

void cryptographicAgent::showUI(int mode){
    connect(ui->cofirmButton, &QPushButton::clicked, this, [&]() {
        hasPassword = true;
        emit passwordSet();  // Notify that password is set
        // this->hide();  // Close UI
    });
    switch (mode) {
    case 1:
        ui->confrimPassText->hide();
        ui->passwordConfirmField->hide();
        this->show();

        break;
    case 2:
        this->show();
        break;

    default:
        break;
    }
}

void cryptographicAgent::on_cofirmButton_clicked()
{
    QString pass=ui->passwordField->text();
    if( !ui->passwordConfirmField->isHidden() ){
        if(ui->passwordField->text() != ui->passwordConfirmField->text() ){
            QMessageBox::warning(this, "Error", "Passwords do not match!!");
            return;
        }
    }

    if( hashResult != hashPassword(pass).toStdString() ){
        QMessageBox::warning(this, "Error", "Wrong Password!!");
        return;
    }
    else{
        storePassword( ui->passwordField->text() );
        this->hide();
    }
}

