#include "licenseservice.h"
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
    m_tunnelSrv = TunnelService::instance();
    m_license = LicenseService::instance();
}

QTunnel::~QTunnel()
{
    m_tunnelSrv->closeConnection();
}

void QTunnel::connect()
{
    // get conf file from license file
    QVariantMap configObj;
    long res = m_license->getConfig(configObj);

    if (res != 1) {
        emit errorRaised("Couldn't create configuration", "Make sure the license file is valid");
    }

    res = m_tunnelSrv->connect(configObj);

    if (res != 1) {
        emit errorRaised("Couldn't create tunnel", "Make sure the license file is valid");
    }

    QTimer::singleShot(1000, this, SLOT(onFinishConnecting()));
}

void QTunnel::onFinishConnecting()
{
    long res = m_tunnelSrv->checkConnection();

    if (res == 1) {
        emit connected();
    } else {
        emit errorRaised("Failed to connect to remote server!", "Make sure the program is running with administrator rights ");
    }

    m_tunnelSrv->finish();
}

