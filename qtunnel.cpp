#include "licenseservice.h"
#include "qtunnel.h"
#include <QCborStreamWriter>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QTimer>
#include <QPromise>
#include <QtConcurrent/QtConcurrent>


QTunnel::QTunnel(QObject *parent) : QObject(parent)
{
    m_tunnelSrv = TunnelService::instance();
    m_license = LicenseService::instance();
}

QTunnel::~QTunnel()
{
    waitForThreads(5000);
    m_tunnelSrv->closeConnection();
}

void QTunnel::waitForThreads(int millisecondsToWait)
{
    if (!m_watcher) return;

    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );

    while (QTime::currentTime() < dieTime) {
        if (m_watcher->isFinished()) {
            break;
        }
    }

    delete m_watcher;
    m_watcher = nullptr;
}

long QTunnel::connectTo(QVariantMap configObj)
{
    m_isConnected = false;
    long res = m_tunnelSrv->connect(configObj);

    if (res != 1) {
        return res;
    }

    waitForFinishConnecting();

    return m_isConnected ? 1 : 0;
}

void QTunnel::connect()
{
    waitForThreads(5000);

    Q_ASSERT(m_watcher == nullptr);

    auto future = QtConcurrent::run(&QTunnel::th_connect, this);

    m_watcher = new QFutureWatcher<void>();

    m_watcher->setFuture(future);
}

void QTunnel::waitForFinishConnecting()
{
    long res = m_tunnelSrv->checkConnection();

    if (res == 1) {
        m_isConnected = true;
    } else {
        m_tunnelSrv->closeConnection();
        m_isConnected = false;
    }

    m_tunnelSrv->finish();
}

void QTunnel::th_connect()
{
    QVariantMap configObj;
    long res = m_license->getServerConfig(configObj, false);
    if (res == 1) {
        res = connectTo(configObj);
    }

    if (res != 1) {
        QVariantMap reserveConfigObj;
        res = m_license->getServerConfig(reserveConfigObj, true); // reserve scheme
        Q_ASSERT(res == 1);
        res = connectTo(configObj);
    }

    if (res == 1 && m_isConnected) {
        emit connected();
    } else {
        emit errorRaised("Couldn't create tunnel", "Make sure the license is valid");
    }

    return;
}

