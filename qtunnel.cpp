#include "qtunnel.h"
#include <QCborStreamWriter>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QTcpSocket>
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


long QTunnel::createConfigFile()
{
    LicenseService license;

    QVariantMap configObj;
    long res = license.getConfig(configObj);
    if (res != 1) {
        return res;
    }

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

    res = generateContent(configObj, outStream);
    configFile.close();

    return res;
}

long QTunnel::generateContent(const QVariantMap &configOb, QTextStream& stream)
{
    QStringList interfaceKeys;
    interfaceKeys << "PrivateKey" << "Address";
    stream << "[Interface]" << Qt::endl;

    for (QString key: interfaceKeys) {
        if (!configOb.contains(key)) {
            qWarning() << "Config file is not valid! Key " << key << "is absent";
            return 0;
        }
        stream << key << " = " << configOb[key].toString() << Qt::endl;;
    }

    QStringList peerKeys;
    peerKeys << "PublicKey" << "AllowedIPs" << "Endpoint";
    stream << "[Peer]" << Qt::endl;

    for (QString key: peerKeys) {
        if (!configOb.contains(key)) {
            qWarning() << "Config file is not valid! Key " << key << "is absent";
            return 0;
        }
        stream << key << " = " << configOb[key].toString() << Qt::endl;;
    }

    return 1;
}


void QTunnel::connect()
{
    if (m_proc != nullptr) {
        return;
    }

    // get conf file from license file
    long res = createConfigFile();
    if (res != 1) {
        emit errorRaised("Couldn't create configuration", "Make sure the license file is valid");
    }

    QProcess proc ;
    m_proc = new QProcess();

    QStringList args;
    args << "/installtunnelservice";

    args << m_configFile;

    QString configPath = QCoreApplication::applicationDirPath() + "\\config";
    QString processCmd = configPath + "\\vpn.exe";
    m_proc->start(processCmd, args);
    m_proc->waitForFinished(5000);

    QTimer::singleShot(1000, this, SLOT(checkConnection()));

    // QProcess::ProcessState state = m_proc->state();
    // QProcess::ProcessError error = m_proc->error();
    qDebug() << m_proc->readAllStandardOutput();
    qDebug() << m_proc->readAllStandardError();
}

QString QTunnel::lastError()
{
    return 0;
}

void QTunnel::checkConnection()
{
    LicenseService license;
    QVariantMap configObj;
    long res = license.getConfig(configObj);
    if (res != 1) {
        emit errorRaised("Failed to connect to remote server!", "The configuration is not valid");
        return;
    }

    QString checkIP = configObj["Server"].toString();
    QTcpSocket tcpSocket;
    QHostAddress mainVpnHost(checkIP);

    tcpSocket.connectToHost(mainVpnHost, 22);
    if (tcpSocket.waitForConnected(7000)) {
        emit connected();
    } else {
        emit errorRaised("Failed to connect to remote server!", "Make sure the program is running with administrator rights ");
    }

    QFile file (m_configFile);
    file.remove();
}

void QTunnel::deleteConfigFile()
{
    QFile file (m_configFile);
    file.remove();
}
