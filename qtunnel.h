#ifndef QTUNNEL_H
#define QTUNNEL_H

#include "licenseservice.h"
#include "tunnelservice.h"

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

private:
    void waitForfinishConnecting(QPromise<void> &promise);
private slots:
    void onFinishConnecting();

private:
    TunnelService* m_tunnelSrv;
    LicenseService* m_license;

    bool m_isConnected = false;
};

#endif // QTUNNEL_H
