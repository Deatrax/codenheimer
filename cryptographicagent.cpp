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

    strncpy(password, pass.toStdString().c_str() , sizeof(password) - 1);
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
    qDebug() << "Session timed out, Password securely erased.";
    hasPassword=false;
}

QString cryptographicAgent::getAppDataFilePath(const QString& filename) {
    QString appDataPath;

#ifdef _WIN32
    appDataPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/KCATDVWSPJD";

#elif defined(__APPLE__)
    appDataPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/KCATDVWSPJD";
#else
    appDataPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/KCATDVWSPJD";
#endif
    // Ensure filename is safe (no directory traversal)
    QString safeFilename = QFileInfo(filename).fileName();

    // Construct full path
    QString fullPath = QDir::cleanPath(appDataPath + "/" + safeFilename);

    qDebug()<<"File path before repair: "<<fullPath;

    // Replace "Local" with "Roaming" to get the correct path
    fullPath.replace("Local", "Roaming");

    qDebug() << "Corrected AppDataPath:" << appDataPath;

    // Ensure compatibility with OpenSSL (use forward slashes)
    return QDir::toNativeSeparators(fullPath);
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

int cryptographicAgent::encryptToFile(QString fileName, QString data){


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

    QString filePath = getAppDataFilePath(fileName);
    qDebug()<<"the filepath that was generated: "<<filePath;

    try {
        encryptStringToFile(dataToEncrypt, filePath, key, iv);
        //QMessageBox::information(this, "Success", "File encrypted successfully.");
    } catch (const std::runtime_error &e) {
        QMessageBox::warning(this, "Error", e.what());
        return -3;
    }
    return 0;
}

QString cryptographicAgent::decryptFromFile(QString fileName)
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

    QString filePath = getAppDataFilePath(fileName);

    try {
        decryptedContent = decryptFileToString(filePath, key, iv);
        // ui->FileContents->setPlainText(decryptedContent); // Display decrypted content
        //QMessageBox::information(this, "Success", "File decrypted successfully. CONTENTS:"+decryptedContent);
    } catch (const std::runtime_error &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
    return decryptedContent;
}

std::string cryptographicAgent::setPassFirstTime()
{
    firstTime=true;
    QEventLoop loop;
    connect(this, &cryptographicAgent::passwordSet, &loop, &QEventLoop::quit);
    showUI(2);
    loop.exec();
    return hashResult=hashPassword(ui->passwordField->text()).toStdString();
}

QString cryptographicAgent::hashPassword(const QString& pass){
    return QString(QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Sha256).toHex()) ;
}

void cryptographicAgent::showUI(int mode){

    // connect(ui->cofirmButton, &QPushButton::clicked, this, [&]() {
    //       // Notify that password is set
    //     // this->hide();  // Close UI
    // });

    ui->passwordConfirmField->clear();
    ui->passwordField->clear();
    switch (mode) {
    case 1:  //for normal operation
        ui->confrimPassText->hide();
        ui->passwordConfirmField->hide();
        this->show();

        break;
    case 2: //for setting a new password
        ui->confrimPassText->show();
        ui->passwordConfirmField->show();
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
    if(firstTime){
        storePassword( ui->passwordField->text() );
        this->hide();
        return;
    }
    else if( hashResult != hashPassword(pass).toStdString()  ){  //CRITIAL password hash verification
        QMessageBox::warning(this, "Error", "Wrong Password!!");
        return;
    }
    else{
        storePassword( ui->passwordField->text() );
        emit passwordSet();
        this->hide();
    }
}

void cryptographicAgent::setHash(std::string str){
    hashResult=str;
}

std::string cryptographicAgent::getHash(){
    return hashResult;
}


void cryptographicAgent::closeEvent(QCloseEvent *event) {
    emit windowClosed();
    event->accept();  // Accept the close event
}



/// @brief 
/// @return 
bool cryptographicAgent::authenticate(){
    showUI(1);
    bool ok = false;

    // Create an event loop to halt execution
    QEventLoop loop;
    // Connect a signal (e.g., button clicked) to exit the loop when the user provides input
    connect(ui->cofirmButton, &QPushButton::clicked, &loop, &QEventLoop::quit);
    // Connect window close to exit loop
    connect(this, &cryptographicAgent::windowClosed, &loop, &QEventLoop::quit);// Run the event loop (halts execution here until quit() is called)
    loop.exec();
    
    // // Check if the window is still open (if closed, return false)
    // if (!this->isVisible()) {
    //     qDebug()<<"manual window close detected";
    //     return false;
    // }

    this->hide();
    QString pass = ui->passwordField->text();

    if (hashPassword(pass).toStdString() == hashResult) {  
        ok = true;
    }

    return ok;
}