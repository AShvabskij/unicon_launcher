#ifndef QTUNNEL_H
#define QTUNNEL_H

#include "licenseservice.h"
#include "tunnelservice.h"

#include <QFutureWatcher>
#include <QObject>
#include <QPromise>

class QTunnel : public QObject
{
    Q_OBJECT
public:
    explicit QTunnel(QObject *parent = nullptr);
    ~QTunnel();

    Q_INVOKABLE void connect();

signals:
    void connected();
    void errorRaised(QString errorTxt, QString informativeError);

private slots:
    void waitForFinishConnecting();
    void th_connect();

private:
    long connectTo(QVariantMap configObj);
    void waitForThreads(int millisecondsToWait);

    TunnelService* m_tunnelSrv;
    LicenseService* m_license;

    bool m_isConnected = false;

    QFutureWatcher<void>* m_watcher = nullptr;
};

#endif // QTUNNEL_H
