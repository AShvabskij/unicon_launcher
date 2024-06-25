#include "qtunnel.h"
#include <QCborStreamWriter>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QTimer>

QTunnel::QTunnel(QObject *parent) : QObject(parent)
{

}

QTunnel::~QTunnel()
{
    if (m_proc == nullptr) {
        return;
    }

    QStringList args;
    args << "/uninstalltunnelservice";
    args << "unicon";

    QString configPath = QCoreApplication::applicationDirPath() + "\\config";

    QString processCmd = configPath + "\\vpn.exe";
    m_proc->startDetached(processCmd, args);

    m_proc->close();
    delete m_proc;
}

void QTunnel::createLicFile()
{
    QJsonObject res;
    QJsonObject config;

    QJsonArray nodesArr;

    config["PrivateKey"] = "oIsGee+Dr8zECo/hs+7LIvJO7b+T6R8f4mdBLb7fynE=";
    config["Address"] = "10.9.0.2/24";

    config["PublicKey"] = "Fs5WF+c1G0Bh2x/ur33wmw33zY3Id9FR/mQQc7XaiSA=";
    config["AllowedIPs"] = "10.9.0.0/24";
    config["Endpoint"] = "92.118.113.157:51820";

    QJsonObject nodeObj1;
    nodeObj1["region"] = "Katar";
    nodeObj1["name"] = "Starter 2";
    nodeObj1["address"] = "Katar techo park";
    nodeObj1["owner"] = "Totem Ltd";
    nodeObj1["ipAddress"] = "10.9.0.5";

    QJsonObject nodeObj2;
    nodeObj2["region"] = "Munhen";
    nodeObj2["name"] = "RND Lab";
    nodeObj2["address"] = "Germany Hermann-Ritter-Str. 110 Bremen";
    nodeObj2["owner"] = "IGEL Electric GmbH";
    nodeObj2["ipAddress"] = "10.9.0.22";

    nodesArr << nodeObj1 << nodeObj2;

    res["nodes"] = nodesArr;
    res["config"] = config;

    QString path = QCoreApplication::applicationDirPath() + "\\config";
    QString baseFileName = "license";

    QString datFile = path + "/" + baseFileName + ".dat";
    saveObj(datFile, res, false);
}

long QTunnel::createConfigFile()
{

    QString path = QCoreApplication::applicationDirPath() + "\\config";
    QString datFileName = path + "/" + "license" + ".dat";
    QFile datFile( datFileName );

    if(!datFile.open( QIODevice::ReadOnly | QIODevice::Text ))
    {
        QTextStream(stdout) << "file open failed: " << datFileName << Qt::endl;
        return 0;
    }

    QTextStream inStream( &datFile );
    QString content = inStream.readAll();
    datFile.close();

    QJsonDocument d = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject obj = d.object();
    QJsonObject configObj = obj.value("config").toObject();

    QString configPath = QCoreApplication::applicationDirPath() + "\\config";
    m_configFile = configPath + "\\unicon.conf";

    QFile configFile( m_configFile );

    if(!configFile.open( QIODevice::NewOnly))
    {
        QTextStream(stdout) << "file open failed: " << m_configFile << Qt::endl;
        return 0;
    }

    QTextStream outStream( &configFile );
    outStream.setEncoding(QStringConverter::Utf8);

    generateContent(configObj, outStream);
    configFile.close();

    return 1;
}

long QTunnel::generateContent(const QJsonObject& sourceObj, QTextStream& stream)
{
    stream << "[Interface]" << Qt::endl;
    stream << "PrivateKey = ";
    stream << sourceObj["PrivateKey"].toString() << Qt::endl;
    stream << "Address = ";
    stream << sourceObj["Address"].toString() << Qt::endl;

    stream << Qt::endl;
    stream << "[Peer]" << Qt::endl;
    stream << "PublicKey = ";
    stream << sourceObj["PublicKey"].toString() << Qt::endl;
    stream << "AllowedIPs = ";
    stream << sourceObj["AllowedIPs"].toString() << Qt::endl;
    stream << "Endpoint = ";
    stream << sourceObj["Endpoint"].toString() << Qt::endl;

    return 1;
}

long QTunnel::saveObj(const QString fileName, const QJsonObject &obj, bool useBinaryFormat)
{
    long res = 1;

    QFile file( fileName );

    if (useBinaryFormat) {
        if( !file.open( QIODevice::WriteOnly |  QIODevice::Truncate ) ) {
            QTextStream(stdout) << "File open failed: " << fileName << Qt::endl;
            return 0;
        }

        QCborStreamWriter writer(&file);

        QCborValue cborValue = QCborValue::fromJsonValue(obj);
        QByteArray bytes = cborValue.toCbor(QCborValue::UseFloat);

        writer.append(bytes); // serialize the QCborValue to the file

    } else {
        if( !file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
        {
            QTextStream(stdout) << "File open failed: " << fileName << Qt::endl;
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

void QTunnel::connect()
{
    if (m_proc != nullptr) {
        return;
    }

    // get conf file from license file

//  createLicFile();
    createConfigFile();

    QProcess proc ;
    m_proc = new QProcess();


    QStringList args;
    args << "/installtunnelservice";

    args << m_configFile;

    QString configPath = QCoreApplication::applicationDirPath() + "\\config";
    QString processCmd = configPath + "\\vpn.exe";
    m_proc->start(processCmd, args);
    m_proc->waitForFinished(5000);

    QTimer::singleShot(5000, this, SLOT(deleteConfigFile()));

    // QProcess::ProcessState state = m_proc->state();
    // QProcess::ProcessError error = m_proc->error();
    qDebug() << m_proc->readAllStandardOutput();
    qDebug() << m_proc->readAllStandardError();
}

QString QTunnel::lastError()
{
    return 0;
}

void QTunnel::deleteConfigFile()
{

    QFile file (m_configFile);
    file.remove();

}
