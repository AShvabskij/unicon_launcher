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

    config["PrivateKey"] = "oIsGee+Dr8zECo/hs+7LIvJO7b+T6R8f4mdBLb7fynE=";
    config["Address"] = "10.9.0.2/24";
    config["Server"] = "10.9.0.1";

    config["PublicKey"] = "Fs5WF+c1G0Bh2x/ur33wmw33zY3Id9FR/mQQc7XaiSA=";
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
    saveObj(datFile, res, false);
}

long LicenseService::saveObj(const QString fileName, const QJsonObject &obj, bool useBinaryFormat)
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

long LicenseService::getConfig(QVariantMap& retDat)
{
    QString path = QCoreApplication::applicationDirPath() + "\\config";
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

    if (!obj.contains("config")) {
        qWarning() << "The license file is not valid! Key config is absent";
        return 0;
    }

    QJsonObject configObj = obj.value("config").toObject();

    retDat = configObj.toVariantMap();
    return 1;
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
