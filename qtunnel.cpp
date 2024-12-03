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
    m_tunnelSrv->closeConnection();
}

void QTunnel::connect()
{
    if (m_isConnected == true) {
        auto future = QtConcurrent::run(&QTunnel::waitForfinishConnecting, this);
        return;
    }

    // get conf file from license file
    QVariantMap configObj;
    long res = m_license->getServerConfig(configObj, false);

    if (res == 1) {
        res = m_tunnelSrv->connect(configObj);
    }

    if (res != 1) {
        emit errorRaised("Couldn't create tunnel", "Make sure the license is valid");
    }

    QTimer::singleShot(100, this, SLOT(onFinishConnecting()));
}

void QTunnel::waitForfinishConnecting(QPromise<void>& promise)
{
    long res = m_tunnelSrv->checkConnection();

    if (res == 1) {
        m_isConnected = true;
    } else {
        m_tunnelSrv->closeConnection();
        m_isConnected = false;
    }

    m_tunnelSrv->finish();
    promise.finish();
}

void QTunnel::onFinishConnecting()
{
    auto future = QtConcurrent::run(&QTunnel::waitForfinishConnecting, this);

    QFutureWatcher<void>* watcher = new QFutureWatcher<void>();
    watcher->setFuture(future);

    QObject::connect(watcher, &QFutureWatcher<void>::finished, [this, watcher]() {
        if (m_isConnected) {
            emit connected();
        } else {

            // Let's try reserve scheme
            QVariantMap configObj;
            long res = m_license->getServerConfig(configObj, true);

            if (res == 1) {
                res = m_tunnelSrv->connect(configObj);
            }

            if (res != 1) {
                emit errorRaised("Couldn't create tunnel", "Make sure the license is valid");
            }

            auto future = QtConcurrent::run(&QTunnel::waitForfinishConnecting, this);
            delete watcher;
            QFutureWatcher<void>* watcher2 = new QFutureWatcher<void>();
            watcher2->setFuture(future);
            QObject::connect(watcher, &QFutureWatcher<void>::finished, [this, watcher2]() {
                if (m_isConnected) {
                    emit connected();
                } else {
                    emit errorRaised("Server is not available!", "Please try again");
                }
                delete watcher2;

            });
        }

        delete watcher;
    });

}

