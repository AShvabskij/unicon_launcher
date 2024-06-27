#include "tunnelservice.h"
#include "qcontainerfwd.h"

#include <qcoreapplication.h>
#include <qfile.h>
#include <qtcpsocket.h>

TunnelService::TunnelService() {}

long TunnelService::createConfigFile(const QVariantMap &configObj)
{

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

    long res = generateContent(configObj, outStream);
    configFile.close();

    return res;
}

long TunnelService::generateContent(const QVariantMap &configOb, QTextStream& stream)
{
    QStringList interfaceKeys;
    interfaceKeys << "PrivateKey" << "Address";
    stream << "[Interface]" << Qt::endl;

    for (QString key: interfaceKeys) {
        if (!configOb.contains(key)) {
            qWarning() << "Config is not valid! Key " << key << "is absent";
            return 0;
        }
        stream << key << " = " << configOb[key].toString() << Qt::endl;;
    }

    QStringList peerKeys;
    peerKeys << "PublicKey" << "AllowedIPs" << "Endpoint";
    stream << "[Peer]" << Qt::endl;

    for (QString key: peerKeys) {
        if (!configOb.contains(key)) {
            qWarning() << "Config is not valid! Key " << key << "is absent";
            return 0;
        }
        stream << key << " = " << configOb[key].toString() << Qt::endl;;
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

    QProcess proc;

    QStringList args;
    args << "/installtunnelservice";

    QString configPath = QCoreApplication::applicationDirPath() + "\\config";
    QString configFile = configPath + "\\unicon.conf";

    args << configFile;

    QString processCmd = configPath + "\\vpn.exe";
    proc.start(processCmd, args);
    proc.waitForFinished(5000);

    // QProcess::ProcessState state = m_proc->state();
    // QProcess::ProcessError error = m_proc->error();
    qDebug() << proc.readAllStandardOutput();
    qDebug() << proc.readAllStandardError();
    return 1;
}

long TunnelService::checkConnection(const QVariantMap &configObj)
{
    if (!configObj.contains("Server")) {
        qWarning() << "Config is not valid! Key " << "Server" << "is absent";
        return 0;
    }

    QString checkIP = configObj["Server"].toString();

    QTcpSocket tcpSocket;

    QHostAddress mainVpnHost(checkIP);

    tcpSocket.connectToHost(mainVpnHost, 22);
    if (tcpSocket.waitForConnected(7000)) {
        return 1;
    } else {
        return 0;
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
    proc.startDetached(processCmd, args);

    proc.close();

    return 1;
}

void TunnelService::onConnected()
{
    QFile file (m_configFile);
    file.remove();
}
