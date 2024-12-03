#include "licenseservice.h"

#include <qcoreapplication.h>
#include <QFile>
#include <QJsonArray>
#include <QCborStreamWriter>
#include <QJsonDocument>

LicenseService::LicenseService() {}

void LicenseService::createLicFile()
{
    QJsonObject res;
    QJsonObject config;

    QJsonArray nodesArr;

    config["AppKey"] = "oIsGee+Dr8zECo/hs+7LIvJO7b+T6R8f4mdBLb7fynE=";
    config["Address"] = "10.9.0.2/24";
    config["ServerIP"] = "10.9.0.1";

    config["ServerKey"] = "Fs5WF+c1G0Bh2x/ur33wmw33zY3Id9FR/mQQc7XaiSA=";
    config["AllowedIPs"] = "10.9.0.0/24";
    config["Endpoint"] = "92.118.113.157:51820";
    config["Endpoint"] = "92.118.113.157:51820";

    QJsonObject nodeObj1;
    nodeObj1["region"] = "Katar";
    nodeObj1["name"] = "Starter 1";
    nodeObj1["address"] = "Katar techo park";
    nodeObj1["owner"] = "Totem Ltd";
    nodeObj1["ipAddress"] = "10.9.0.4";

    QJsonObject nodeObj2;
    nodeObj2["region"] = "Katar";
    nodeObj2["name"] = "Starter 2";
    nodeObj2["address"] = "Katar techo park";
    nodeObj2["owner"] = "Totem Ltd";
    nodeObj2["ipAddress"] = "10.9.0.5";

    QJsonObject nodeObj3;
    nodeObj3["region"] = "Munhen";
    nodeObj3["name"] = "RND Lab";
    nodeObj3["address"] = "Germany Hermann-Ritter-Str. 110 Bremen";
    nodeObj3["owner"] = "IGEL Electric GmbH";
    nodeObj3["ipAddress"] = "10.9.0.5";

    QJsonObject nodeObj4;
    nodeObj4["region"] = "Munhen";
    nodeObj4["name"] = "Starter 1";
    nodeObj4["address"] = "Germany Hermann-Ritter-Str. 110 Bremen";
    nodeObj4["owner"] = "IGEL Electric GmbH";
    nodeObj4["ipAddress"] = "10.9.0.11";

    nodesArr << nodeObj1 << nodeObj2 << nodeObj3 << nodeObj4;

    res["nodes"] = nodesArr;
    res["config"] = config;

    QString path = QCoreApplication::applicationDirPath() + "\\config";
    QString baseFileName = "license";

    QString datFile = path + "/" + baseFileName + ".dat";
    save(datFile, res, false);
}

void LicenseService::deleteLicFile()
{
    QString path = QCoreApplication::applicationDirPath() + "\\config";
    QString datFileName = path + "/" + "license" + ".dat";
    QFile datFile( datFileName );
    datFile.remove();
}

long LicenseService::save(const QString fileName, const QJsonObject &obj, bool useBinaryFormat)
{
    long res = 1;

    QFile file( fileName );

    if (useBinaryFormat) {
        if( !file.open( QIODevice::WriteOnly |  QIODevice::Truncate ) ) {
            qWarning() << "File open failed: " << fileName << Qt::endl;
            return 0;
        }

        QCborStreamWriter writer(&file);

        QCborValue cborValue = QCborValue::fromJsonValue(obj);
        QByteArray bytes = cborValue.toCbor(QCborValue::UseFloat);

        writer.append(bytes); // serialize the QCborValue to the file

    } else {
        if( !file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
        {
            qWarning() << "File open failed: " << fileName << Qt::endl;
            return 0;
        }
        QJsonDocument doc(obj);
        QByteArray bytes = doc.toJson(QJsonDocument::Indented);

        QTextStream iStream( &file );
        iStream.setEncoding(QStringConverter::Latin1);
        //      iStream.setCodec( "utf-8" );
        iStream << bytes;
    }

    file.close();
    return res;
}

long LicenseService::load(QJsonObject& retObj)
{
    QString path = QCoreApplication::applicationDirPath() + "\\config";
    QString datFileName = path + "/" + "license" + ".dat";
    QFile datFile( datFileName );

    if(!datFile.open( QIODevice::ReadOnly | QIODevice::Text ))
    {
        qWarning() << "file open failed: " << datFileName << Qt::endl;
        return -2;
    }

    QTextStream inStream( &datFile );
    QString content = inStream.readAll();
    datFile.close();

    QJsonDocument d = QJsonDocument::fromJson(content.toUtf8());
    retObj = d.object();

    return 1;
}

long LicenseService::getConfig(QVariantMap& retDat)
{
    QJsonObject obj;
    long res = load(obj);
    if (res != 1) return res;

    if (!obj.contains("config")) {
        qWarning() << "The license file is not valid! Config section is not found";
        return 0;
    }

    QJsonObject configObj = obj.value("config").toObject();

    retDat = configObj.toVariantMap();
    return 1;
}

bool LicenseService::checkConfig(const QVariantMap &dat)
{
    QStringList keys;
    keys << "Number" << "AppKey";

    for (QString key : keys) {
        if (!dat.contains(key)) {
            qWarning() << "The license is not valid! Key is absent:" << key;
            return false;
        }
    }

    return true;
}

long LicenseService::getServerConfig(QVariantMap& retDat, bool useReserveConfig)
{
    QJsonObject obj;
    long res = load(obj);
    if (res != 1) return res;

    const QString key = useReserveConfig ? "reserve" : "server";

    if (!obj.contains(key)) {
        qWarning() << "The license file is not valid! Server section is not found";
        return 0;
    }

    QJsonObject configObj = obj.value(key).toObject();

    retDat = configObj.toVariantMap();
    return 1;
}

bool LicenseService::checkServerConfig(const QVariantMap &dat)
{
    QStringList keys;
    keys << "Address" << "AllowedIPs" << "Endpoint" << "VPKey" << "ServerKey" << "ServerIP";

    for (QString key : keys) {
        if (!dat.contains(key)) {
            qWarning() << "The license is not valid! Key is absent:" << key;
            return false;
        }
    }

    return true;
}

long LicenseService::getNodes(QVariantList& retDat)
{
    QString path = QCoreApplication::applicationDirPath() + "/config";
    QString datFileName = path + "/" + "license" + ".dat";
    QFile datFile( datFileName );

    if(!datFile.open( QIODevice::ReadOnly | QIODevice::Text ))
    {
        qWarning() << "file open failed: " << datFileName << Qt::endl;
        return 0;
    }

    QTextStream inStream( &datFile );
    QString content = inStream.readAll();
    datFile.close();

    QJsonDocument d = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject obj = d.object();
    if (!obj.contains("nodes")) {
        qWarning() << "The license file is not valid! Key nodes is absent";
        return 0;
    }

    QJsonArray nodesObj = obj.value("nodes").toArray();
    if (nodesObj.isEmpty()) {
        qWarning() << "The license file is not valid! Key nodes is empty";
        return 0;
    }

    foreach (const QJsonValue &item, nodesObj) {
        retDat << item.toObject().toVariantMap();
    }

    return 1;
}

long LicenseService::saveFile(QString licFilePath)
{
    QString path = QCoreApplication::applicationDirPath() + "\\config";
    QString baseFileName = "license";

    QString datFilePath = path + "/" + baseFileName + ".dat";

    licFilePath = removeFilePrefix(licFilePath);
    long res = replaceFile(licFilePath, datFilePath);
    return res;
}

long LicenseService::saveKey(const QString& key)
{
    QString path = QCoreApplication::applicationDirPath() + "\\config";
    QString baseFileName = "appkey";

    QString filePath = path + "/" + baseFileName + ".dat";

    QFile destinationFile(filePath);

    if (destinationFile.exists()) {
        if (!destinationFile.remove()) {
            qWarning() << "Failed to remove existing destination file:" << filePath;
            return 0;
        }
    }

    if (!destinationFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for writing";
        return 0;
    }

    QTextStream out(&destinationFile);
    QString textToWrite = key;
    out << textToWrite;
    destinationFile.close();

    return 1;
}

long LicenseService::getKey(QString& key)
{
    QString path = QCoreApplication::applicationDirPath() + "\\config";
    QString baseFileName = "appkey";

    QString datFilePath = path + "/" + baseFileName + ".dat";

    QFile sourcefile(datFilePath);

    if (!sourcefile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for reading";
        return 0;
    }

    QTextStream in(&sourcefile);
    QString fileContent = in.readLine();
    sourcefile.close();

    key = fileContent;

    return 1;
}

long LicenseService::deleteKey()
{
    QString path = QCoreApplication::applicationDirPath() + "\\config";
    QString baseFileName = "appkey";

    QString keyFilePath = path + "/" + baseFileName + ".dat";

    QFile file(keyFilePath);

    if (file.exists()) {
        if (!file.remove()) {
            qWarning() << "Failed to remove existing key file:" << keyFilePath;
            return 0;
        }
    }

    return 1;
}

long LicenseService::replaceFile(const QString &sourceFilePath, const QString &destinationFilePath)
{
    QFile sourceFile(sourceFilePath);
    QFile destinationFile(destinationFilePath);

    if (!sourceFile.exists()) {
        qWarning() << "Source file does not exist:" << sourceFilePath;
        return 0;
    }

    if (destinationFile.exists()) {
        if (!destinationFile.remove()) {
            qWarning() << "Failed to remove existing destination file:" << destinationFilePath;
            return 0;
        }
    }

    if (!sourceFile.copy(destinationFilePath)) {
        qWarning() << "Failed to copy file from" << sourceFilePath << "to" << destinationFilePath;
        return 0;
    }

    return 1;
}

QString LicenseService::removeFilePrefix(const QString &filePath) {
    const QString prefix = "file:///";
    if (filePath.startsWith(prefix)) {
        return filePath.mid(prefix.length());
    }
    return filePath;
}
