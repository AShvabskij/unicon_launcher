#include "tunnelservice.h"
#include "qcontainerfwd.h"

#include <qcoreapplication.h>
#include <qfile.h>
#include <qtcpsocket.h>

TunnelService::TunnelService() {}

QString TunnelService::genPrivateKey()
{
    QString configPath = QCoreApplication::applicationDirPath() + "\\config";

    QProcess proc;

    QStringList args;
    args << "genkey";

    QString processCmd = configPath + "\\wg.exe";
    proc.start(processCmd, args);
    proc.waitForFinished(5000);

    QString result = proc.readAllStandardOutput();
    qDebug() << proc.readAllStandardError();

    int linePos = result.indexOf('=');

    if (linePos != -1) {
        result = result.left(linePos + 1);
    }

    return result;
}

QString TunnelService::genPublicKey(QString privateKey)
{
    QString configPath = QCoreApplication::applicationDirPath() + "\\config";

    QProcess proc;

    QStringList args;
    args << "pubkey";
    args << "<";
    args << configPath + "\\privatekey";
    args << ">";
    args << configPath + "\\publickey";

    QString processCmd = configPath + "\\wg.exe";
    proc.start(processCmd, args);
    proc.waitForFinished(5000);

    QString result = proc.readAllStandardOutput();
    qDebug() << proc.readAllStandardError();

    int linePos = result.indexOf('=');

    if (linePos != -1) {
        result = result.left(linePos + 1);
    }

    return result;
}

long TunnelService::createConfigFile(const QVariantMap &configObj)
{

    QString configPath = QCoreApplication::applicationDirPath() + "\\config";
    m_configFile = configPath + "\\unicon.conf";

    QFile configFile( m_configFile );

    configFile.remove();
    if(!configFile.open( QIODevice::NewOnly))
    {
        QTextStream(stdout) << "file open failed: " << m_configFile << Qt::endl;
        return 0;
    }

    QTextStream outStream( &configFile );
    outStream.setEncoding(QStringConverter::Utf8);

    long res = generateContent(configObj, outStream);
    configFile.close();

    return res;
}

long TunnelService::generateContent(const QVariantMap &configObj, QTextStream& stream)
{
    QMap<QString, QString> interfaceKeys;
    interfaceKeys.insert("VPKey", "PrivateKey");
    interfaceKeys.insert("Address", "Address");
    stream << "[Interface]" << Qt::endl;

    for (QString key: interfaceKeys.keys()) {
        if (!configObj.contains(key)) {
            qWarning() << "Config is not valid! Key " << key << "is not found";
            return 0;
        }
        stream << interfaceKeys[key] << " = " << configObj[key].toString() << Qt::endl;;
    }

    QMap<QString, QString> peerKeys;
    QString privateKey = configObj["PublicKey"].toString();
    peerKeys.insert("ServerKey", "PublicKey");
    peerKeys.insert("AllowedIPs", "AllowedIPs");
    peerKeys.insert("Endpoint", "Endpoint");

    stream << "[Peer]" << Qt::endl;

    for (QString key: peerKeys.keys()) {
        if (!configObj.contains(key)) {
            qWarning() << "Config is not valid! Key " << key << "is not found";
            return 0;
        }
        stream << peerKeys[key] << " = " << configObj[key].toString() << Qt::endl;
    }

    return 1;
}

long TunnelService::connect(const QVariantMap &configObj)
{
    long res = createConfigFile(configObj);
    if (res != 1) {
        return res;
    }

    if (m_proc != nullptr) {
        return 0;
    }

    if (!(configObj.contains("ServerIP"))) {
        qWarning() << "Config is not valid! Key " << "ServerIP" << "is not found";
        return 0;
    }

    m_serverIP = configObj["ServerIP"].toString();

    QProcess proc;

    QStringList args;
    args << "/installtunnelservice";

    QString configPath = QCoreApplication::applicationDirPath() + "\\config";
    QString configFile = configPath + "\\unicon.conf";

    args << configFile;

    QString processCmd = configPath + "\\vpn.exe";
    proc.start(processCmd, args);
    proc.waitForFinished(5000);

    QString output = proc.readAllStandardOutput();
    QString errOutput = proc.readAllStandardError();
    if (output != "" || errOutput != "") {
        qWarning() << output;
        qWarning() << errOutput;
    }

    return 1;
}

long TunnelService::checkConnection()
{
    if (m_serverIP == "") {
        qCritical() << "Server IP is not defined!";
        return 0;
    }

    QTcpSocket tcpSocket;

    QHostAddress mainVpnHost(m_serverIP);

    tcpSocket.connectToHost(mainVpnHost, 22);
    if (tcpSocket.waitForConnected(3000)) {
        return 1;
    } else {
        tcpSocket.connectToHost(mainVpnHost, 22);
        if (tcpSocket.waitForConnected(3000)) {
            return 1;
        } else {
            qWarning() << "Server is not available!";
            return 0;
        }
    }
}

long TunnelService::closeConnection()
{

    QProcess proc;

    QStringList args;
    args << "/uninstalltunnelservice";
    args << "unicon";

    QString configPath = QCoreApplication::applicationDirPath() + "\\config";

    QString processCmd = configPath + "\\vpn.exe";
    proc.start(processCmd, args);

    proc.waitForFinished(5000);

    qDebug() << proc.readAllStandardOutput();
    qDebug() << proc.readAllStandardError();

    proc.close();

    return 1;
}

void TunnelService::finish()
{
    QFile file (m_configFile);
    file.remove();
}
