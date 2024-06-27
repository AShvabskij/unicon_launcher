#ifndef QTUNNEL_H
#define QTUNNEL_H

#include "licenseservice.h"
#include "tunnelservice.h"

#include <QObject>

class QTunnel : public QObject
{
    Q_OBJECT
public:
    explicit QTunnel(QObject *parent = nullptr);
    ~QTunnel();

    Q_INVOKABLE void connect();

signals:
    void connected();
    void errorRaised(QString error, QString informativeError);

private slots:
    void finishConnecting();

private:
    TunnelService m_tunnelSrv;
    LicenseService m_license;
};

#endif // QTUNNEL_H
